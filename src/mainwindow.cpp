//  This file is part of SilentEye.
//
//  SilentEye is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  SilentEye is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SilentEye. If not, see <http://www.gnu.org/licenses/>.

#include "mainwindow.h"
#include "controller.h"

#include <audio.h>

namespace SilentEye {

    MainWindow::MainWindow(QString url, QWidget* parent)
        : QMainWindow(parent)
    {
        setupUi(this);
        m_logger = new Logger(this);

        /* center the window */
        QRect rect = QApplication::desktop()->availableGeometry(this);
        move(rect.center() - this->rect().center());

        m_hasMediaLoaded = false;
        tabWidget->clear();
        tabWidget->addTab(new MediaWidget(tabWidget), QString("No media selected"));

        connectSignals();
        if (!url.isEmpty()) {
            newMedia(url);
        }
    }

    MainWindow::~MainWindow()
    {
        if (!m_currentSound.isNull())
            delete m_currentSound;
        delete(contextMenu);
        delete(m_logger);
        delete Controller::instance();
    }

    void MainWindow::addMediaTab(QPointer<Media> md)
    {
        closeTabByPath(md->filePath());

        MediaWidget* w;
        if(!m_hasMediaLoaded)
        {
            w = (MediaWidget*)tabWidget->currentWidget();
            if (md->type() == Media::IMAGE) {
                w->setPixmap(*((Image*)md.data()));
            } else if (md->type() == Media::AUDIO) {
                w->setPixmap(QPixmap(QString::fromUtf8(":/icons/img/audio.png")), false);
                w->showPlayer(true);
                connect(w, SIGNAL(play()), this, SLOT(playMedia()));
                connect(w, SIGNAL(stop()), this, SLOT(stopMedia()));
            } else {
                w->setText(md->shortName());
            }
            tabWidget->setTabText(tabWidget->currentIndex(), md->shortName());
        }
        else
        {
            w = new MediaWidget(tabWidget);
            if (md->type() == Media::IMAGE) {
                w->setPixmap(*((Image*)md.data()));
            } else if (md->type() == Media::AUDIO) {
                w->setPixmap(QPixmap(QString::fromUtf8(":/icons/img/audio.png")), false);
                w->showPlayer(true);
                connect(w, SIGNAL(play()), this, SLOT(playMedia()));
                connect(w, SIGNAL(stop()), this, SLOT(stopMedia()));
            } else {
                w->setText(md->shortName());
            }
            tabWidget->addTab(w, md->shortName());
        }
        w->setContextMenu(contextMenu);
        w->setFilepath(md->filePath());
        closeTabButton->setVisible(true);
        m_mediaMap[md->filePath()]=md;
        setEnabledImageActions(true);
        m_hasMediaLoaded = true;
        tabWidget->setTabToolTip(tabWidget->count()-1, md->filePath());
        tabWidget->setCurrentIndex(tabWidget->count()-1);
    }

    void MainWindow::closeCurrentTab()
    {

        if (!m_currentSound.isNull())
        {
            m_currentSound->stop();
            delete m_currentSound;
        }
        if(tabWidget->count()==1)
        {
            m_hasMediaLoaded = false;
            setEnabledImageActions(false);
            closeTabButton->setVisible(false);
        }
        tabWidget->removeTab(tabWidget->currentIndex());

        if(!m_hasMediaLoaded)
            tabWidget->addTab(new MediaWidget(tabWidget), "No media selected");
    }

    void MainWindow::closeTabByPath(QString path)
    {
        if( !m_mediaMap.contains(path) )
            return;

        int i = 0;
        while(i<tabWidget->count())
        {
            QString currentPath = ((MediaWidget*) tabWidget->widget(i))->filepath();
            if(currentPath == path)
            {
                if (!m_currentSound.isNull())
                {
                    m_currentSound->stop();
                    delete m_currentSound;
                }
                tabWidget->removeTab(i);
                delete(m_mediaMap[path]);
                m_mediaMap.remove(path);
                return;
            }
            i++;
        }
    }

    void MainWindow::setEnabledImageActions(const bool value)
    {
        decodeButton->setEnabled(value);
        actionDecode->setEnabled(value);
        encodeButton->setEnabled(value);
        actionEncode->setEnabled(value);
        propertyButton->setEnabled(value);
        actionProperty->setEnabled(value);
        actionProperty->setEnabled(value);
        actionCloseCurrent->setEnabled(value);
        actionCopy->setEnabled(value);
    }

    void MainWindow::dragEnterEvent(QDragEnterEvent *event)
    {
        if (event->mimeData()->hasUrls() || event->mimeData()->hasText())
            event->acceptProposedAction();
    }

    void MainWindow::dropEvent(QDropEvent *event)
    {
        if(event->mimeData()->hasUrls())
        {
            QString error;
            QList<QUrl> urls = event->mimeData()->urls();
            for(int i=0; i<urls.size(); i++)
            {
                if(urls.at(i).toLocalFile().length()>0)
                {
                    newMedia(urls.at(i).toLocalFile());
                }
                else if (urls.at(i).isValid())
                {
                    if (urls.at(i).scheme() != "http")
                    {
                        error = "unsupported scheme: '"+ urls.at(i).scheme() +"'";
                    } else {
                        setCursor(Qt::WaitCursor);
                        Controller::instance()->httpDownload(urls.at(i));
                    }
                }
                else
                    error = "Invalid url or file doesn't exists";

                if (!error.isEmpty())
                    QMessageBox::warning(this, tr("SilentEye Warning"),
                                         "The dropped object doesn't have valid url ("
                                         + urls.at(i).toString() + ") : " + error);

            }
        }
        else if(event->mimeData()->hasText())
        {
            QUrl url(event->mimeData()->text());
            if(url.toLocalFile().length() > 0)
            {
                newMedia(url.toLocalFile());
            }
            else if (url.scheme() ==  "http")
            {
                setCursor(Qt::WaitCursor);
                Controller::instance()->httpDownload(url);
            }
            else
            {
                QMessageBox::warning(this, tr("SilentEye Warning"),
                                     "The dropped object doesn't have valid url ("
                                     + url.toString() + ")");
            }
        }
        else
        {
            QMessageBox::warning(this, tr("SilentEye Warning"),
                                 "The dropped object doesn't have any url.");
        }
    }

    void MainWindow::httpError(QString url, QString error)
    {
        setCursor(Qt::ArrowCursor);
        m_logger->warning("Cannot download '" + url + "' : " + error);
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("SilentEye error");
        msgBox.setText("Error occured when downloading " + url);
        msgBox.setDetailedText(error);
        msgBox.exec();
    }

    QPointer<Media> MainWindow::currentMedia()
    {
        QString key = ((MediaWidget*) tabWidget->widget(tabWidget->currentIndex()))->filepath();
        if(m_mediaMap.contains(key))
            return m_mediaMap[key];
        else
            return NULL;
    }

    void MainWindow::newMedia(QString url)
    {
        setCursor(Qt::ArrowCursor);
        if(QFile::exists(url))
        {
            if (url.endsWith(".wav", Qt::CaseInsensitive)) {
                QPointer<Media> md;
                try {
                    md = new Audio(url);
                } catch (SilentEyeException e) {
                    QMessageBox msgBox;
                    msgBox.setIcon(QMessageBox::Critical);
                    msgBox.setWindowTitle("SilentEye error");
                    msgBox.setText(e.message());
                    msgBox.setDetailedText(e.details());
                    msgBox.exec();
                    m_logger->warning("Cannot load '" + url + "'\n" + e.message() + ": " + e.details());
                    return;
                }
                addMediaTab(md);
            } else if(url.endsWith(".mp3", Qt::CaseInsensitive) || url.endsWith(".ogg", Qt::CaseInsensitive)
                || url.endsWith(".wma", Qt::CaseInsensitive) || url.endsWith(".flac", Qt::CaseInsensitive)) {
                QMessageBox::warning(this, tr("SilentEye Warning"),
                                     "Cannot load '" + url + "'\nUnsupported audio format, you must load WAVE file.");
                return;
            } else {
                QPointer<Image> img = new Image(url);
                if(img->isNull())
                {
                    QMessageBox::warning(this, tr("SilentEye Warning"),
                                         "Cannot load '" + url + "'\nFile is not a valid media.");
                    return;
                }
                addMediaTab(QPointer<Media>(img));
            }
        }
        else
        {
            m_logger->warning("Cannot load '" + url + "'\n Invalid URL or file doesn't exist !");
            QMessageBox::warning(this, tr("SilentEye Warning"),
                                     "Cannot load '" + url + "'\n Invalid URL or file doesn't exist !");
        }
    }

    void MainWindow::openFile()
    {
        setCursor(Qt::WaitCursor);
        QFileDialog dialog(this, tr("Open Media"));
        dialog.setFilter(tr("Media Files (*.png *.jpg .jpeg *.bmp *.tiff *.tif *.wav)"));
        dialog.setViewMode(QFileDialog::List);
        dialog.setFileMode(QFileDialog::ExistingFiles);
        dialog.setDirectory(Controller::instance()->config.get("output"));
        if(dialog.exec())
        {
            QStringList fileNames = dialog.selectedFiles();
            for(int i=0; i<fileNames.size(); i++)
            {
                newMedia(fileNames.at(i));
            }
        }
        setCursor(Qt::ArrowCursor);
    }

    void MainWindow::paste()
    {
        const QClipboard *clipboard = QApplication::clipboard();
        const QMimeData *mimeData = clipboard->mimeData();

        if (mimeData->hasImage()) {
            QPointer<Image> img = new Image(qvariant_cast<QPixmap>(mimeData->imageData()));
            img->setShortName("pasted_image.img");
            addMediaTab(QPointer<Media>(img));
        } else if (mimeData->hasText()) {
            QUrl url(mimeData->text());
            if(url.toLocalFile().length() > 0)
            {
                newMedia(url.toLocalFile());
            }
            else if (url.scheme() ==  "http")
            {
                setCursor(Qt::WaitCursor);
                Controller::instance()->httpDownload(url);
            }
            else
            {
                QMessageBox::warning(this, tr("SilentEye Warning"),
                                     "The pasted text is not a valid url ("
                                     + url.toString() + ").\nOnly image, local file path and http source are allowed");
            }

        } else {
            QMessageBox::warning(this, tr("SilentEye Warning"),
                                 "Cannot paste clipboard's data: not a supported media!");
        }
    }

    void MainWindow::copy()
    {
        if (m_hasMediaLoaded)
        {
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->clear();
            QPointer<Media> md = currentMedia();
            if (md->type() == Media::IMAGE) {
                Image* img = (Image*) md.data();
                clipboard->setPixmap(*img, QClipboard::Clipboard);
            } else {
                clipboard->setText(md->filePath());
            }
        }
    }

    void MainWindow::playMedia()
    {
        if(m_hasMediaLoaded)
        {
            Media* media = currentMedia();
            if (media->type() == Media::AUDIO) {
                if (!m_currentSound.isNull())
                    delete m_currentSound;
                if (!QSound::isAvailable())
                {
                    QMessageBox::warning(this, tr("SilentEye Warning"),
                                         "Audio device is not ready!");
                }
                else
                {
                    m_currentSound = new QSound(media->filePath());
                    m_currentSound->play();
                }
            }
        }
    }

    void MainWindow::stopMedia()
    {
        if(m_hasMediaLoaded)
        {
            Media* media = currentMedia();
            if (media->type() == Media::AUDIO && !m_currentSound.isNull()) {
                m_currentSound->stop();
            }
        }
    }

    void MainWindow::execPropertyDialog()
    {
        propertyDialog.setMedia(currentMedia());
        propertyDialog.exec();
    }

    void MainWindow::execEncodeDialog()
    {
        if (!m_currentSound.isNull())
        {
            m_currentSound->stop();
            delete m_currentSound;
        }
        encodeDialog.setMedia(currentMedia());
        encodeDialog.exec();
    }

    void MainWindow::execDecodeDialog()
    {
        if (!m_currentSound.isNull())
        {
            m_currentSound->stop();
            delete m_currentSound;
        }
        decodeDialog.setMedia(currentMedia());
        decodeDialog.exec();
    }

    void MainWindow::execUpdateDialog()
    {
        updateDialog.exec();
    }

    void MainWindow::execUrlDialog()
    {
        if (urlDialog.exec())
        {
            setCursor(Qt::WaitCursor);
            Controller::instance()->httpDownload(urlDialog.url());
        }
    }

    void MainWindow::connectSignals()
    {
        // Button actions
        connect(encodeButton, SIGNAL(pressed()), this, SLOT(execEncodeDialog()));
        connect(actionEncode, SIGNAL(triggered()), this, SLOT(execEncodeDialog()));
        connect(propertyButton, SIGNAL(pressed()), this, SLOT(execPropertyDialog()));
        connect(actionProperty, SIGNAL(triggered()), this, SLOT(execPropertyDialog()));
        connect(decodeButton, SIGNAL(pressed()), this, SLOT(execDecodeDialog()));
        connect(actionDecode, SIGNAL(triggered()), this, SLOT(execDecodeDialog()));
        connect(actionCheckUpdate, SIGNAL(triggered()), this, SLOT(execUpdateDialog()));
        connect(actionOpenUrl, SIGNAL(triggered()), this, SLOT(execUrlDialog()));

        // Menu actions
        connect(actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
        connect(actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
        connect(actionPaste, SIGNAL(triggered()), this, SLOT(paste()));
        connect(actionCloseCurrent, SIGNAL(triggered()), this, SLOT(closeCurrentTab()));
        connect(actionPreferences, SIGNAL(triggered()), &preferenceDialog, SLOT(exec()));
        connect(actionAbout, SIGNAL(triggered()), &aboutDialog, SLOT(exec()));

        // Others
        connect(&encodeDialog, SIGNAL(encodedImage(QString)), this, SLOT(newMedia(QString)));
        connect(Controller::instance(), SIGNAL(httpFile(QString)), this, SLOT(newMedia(QString)));
        connect(Controller::instance(), SIGNAL(httpError(QString, QString)), this, SLOT(httpError(QString, QString)));

        contextMenu = new QMenu(this);
        contextMenu->addAction(actionCloseCurrent);
        contextMenu->addSeparator();
        contextMenu->addAction(actionEncode);
        contextMenu->addAction(actionDecode);
        contextMenu->addAction(actionProperty);
        contextMenu->addSeparator();
        contextMenu->addAction(actionCopy);

        // close tab button
        closeTabButton = new QPushButton(this);
        closeTabButton->setIcon(QIcon(":/menu/img/close.png"));
        closeTabButton->setFlat(true);
        tabWidget->setCornerWidget(closeTabButton);
        closeTabButton->setVisible(false);
        connect(closeTabButton, SIGNAL(pressed()), this, SLOT(closeCurrentTab()));
    }

}
