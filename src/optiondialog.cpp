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

#include "optiondialog.h"
#include "controller.h"

#include <imagemoduleinterface.h>
#include <audiomoduleinterface.h>
#include <videomoduleinterface.h>

namespace SilentEye {

    OptionDialog::OptionDialog(QWidget* parent)
        : QDialog(parent)
    {
        setupUi(this);
        m_logger = new Logger(this);

        m_optionWidget = NULL;
        m_checkSize = true;
        m_originalHeight = minimumHeight();
        loadEncryptionTypes();

        setupDialog();
        connectSignals();
    }

    OptionDialog::~OptionDialog()
    {
        if(!m_md.isNull())
            delete(m_md);
        delete m_logger;
    }

    void OptionDialog::setMedia(QPointer<Media> md)
    {
        m_logger->debug("Adding media : " + md->shortName());
        if (m_md.isNull())
            delete m_md;

        // get a media copy
        if (md->type() == Media::IMAGE) {
            m_md = new Image(*((Image*)md.data()));
        } else if (md->type() == Media::AUDIO) {
            m_md = new Audio(*((Audio*)md.data()));
        } else if (md->type() == Media::VIDEO) {
            m_md = new Video(*((Video*)md.data()));
        } else {
            m_md = new Media(*md);
        }

        loadFormats();
    }

    void OptionDialog::selectFolder()
    {
        setCursor(Qt::WaitCursor);
        QFileDialog dialog(this, tr("Save to"));
        dialog.setViewMode(QFileDialog::List);
        dialog.setFileMode(QFileDialog::DirectoryOnly);
        dialog.setDirectory(Controller::instance()->config.get("output"));
        if(dialog.exec())
        {
            QStringList fileNames = dialog.selectedFiles();
            if(fileNames.size()>0)
            {
                if(QFile::exists(fileNames.at(0)))
                    destinationLineEdit->setText(fileNames.at(0));
                else
                    QMessageBox::warning(this, tr("SilentEye Warning"),
                                         fileNames.at(0)+"\n"+
                                         "Selected folder doesn't exists.\n"+
                                         "Please select an other destination!");
            }
        }
        setCursor(Qt::ArrowCursor);
    }

    void OptionDialog::selectExportFolder()
    {
        bool load = this->objectName() == "DecodeDialog";
        setCursor(Qt::WaitCursor);
        QFileDialog dialog(this, (load)?tr("Select exoprted configuration file"):tr("Select destination"));
        dialog.setViewMode(QFileDialog::List);
        if (!load)
            dialog.setFileMode(QFileDialog::DirectoryOnly);
        else
            dialog.setFileMode(QFileDialog::ExistingFiles);
        dialog.setDirectory(Controller::instance()->config.get("output"));
        if(dialog.exec())
        {
            QStringList fileNames = dialog.selectedFiles();
            if(fileNames.size()>0)
            {
                FormatModuleInterface* formatModule = currentFormatModule();
                if (!load)
                {
                    QString fileName = "silenteye-" + QDate::currentDate().toString("yyyyMMdd");
                    QMap<QString, QString> exportMap = currentFormatModule()->encodeWidget()->exportConfiguration();

                    Config conf(fileNames.at(0), fileName, false, this);

                    // Dump configuration into Config object
                    QList<QString> keys = exportMap.keys();
                    foreach(QString key, keys) {
                        conf.set(key, exportMap[key]);
                    }

                    // Save
                    if (conf.save(formatModule->name())) {
                        QMessageBox::information(this, tr("SilentEye Information"),
                                                 "Module configuration has been saved to " + conf.absoluteFilePath() + ".");
                    } else {
                        QMessageBox::information(this, tr("SilentEye Error"),
                                                 "Cannot save module's configuration to " + conf.absoluteFilePath() + ".");
                    }
                }
                else
                {
                    Config conf(QFileInfo(fileNames.at(0)), this);
                    if (conf.isLoaded()) {
                        formatModule->decodeWidget()->loadConfiguration(conf.values());
                        QMessageBox::information(this, tr("SilentEye Information"),
                                             "Module configuration has been loaded.");
                    } else {
                        QMessageBox::information(this, tr("SilentEye Error"),
                                                 "Cannot load module's configuration file: " + conf.absoluteFilePath() + ".");
                    }
                }
            }
        }
        setCursor(Qt::ArrowCursor);
    }

    void OptionDialog::selectFile()
    {
        bool save = this->objectName() == "DecodeDialog";
        setCursor(Qt::WaitCursor);
        QFileDialog dialog(this, (save)?tr("Select destination"):tr("Select file to hide"));
        dialog.setViewMode(QFileDialog::List);
        if (save)
            dialog.setFileMode(QFileDialog::DirectoryOnly);
        else
            dialog.setFileMode(QFileDialog::ExistingFiles);
        dialog.setDirectory(Controller::instance()->config.get("output"));
        if(dialog.exec())
        {
            QStringList fileNames = dialog.selectedFiles();
            if(fileNames.size()>0)
            {
                if (save)
                {
                    QFile file(fileNames.at(0) + "/" + m_filePath);
                    file.open(QFile::WriteOnly);
                    file.write(m_md->encodedData()->toData()->data());
                    file.close();
                    QMessageBox::information(this, tr("SilentEye Information"),
                                             "Decoded file has been saved to:\n" + file.fileName() + ".");
                }
                else
                    addFile(fileNames.at(0));
            }
        }
        setCursor(Qt::ArrowCursor);
    }

    void OptionDialog::addFile(const QString& filename)
    {
        if(QFile::exists(filename))
        {
            QFile file(filename);
            if (file.open(QFile::ReadOnly))
            {
                file.close();
                m_filePath = filename;
                fileNameLabel->setText(QDir::fromNativeSeparators(m_filePath).section("/", -1, -1));
                msgTextEdit->setDisabled(true);
                fileRemoveButton->setEnabled(true);
                msgTextEdit->setText("");
                valueChanged();
            }
            else
                QMessageBox::warning(this, tr("SilentEye Warning"),
                                     filename+"\n"+
                                     "Selected file cannot be read!\n");
        }
        else
            QMessageBox::warning(this, tr("SilentEye Warning"),
                                 filename+"\n"+
                                 "Selected file doesn't exists.\n");
    }

    void OptionDialog::removeFile()
    {
        m_filePath = "";
        fileNameLabel->setText("no file selected");
        fileRemoveButton->setEnabled(false);
        msgTextEdit->setEnabled(true);
        valueChanged();
    }

    void OptionDialog::rejected()
    {
    }

    bool OptionDialog::isDialogReady()
    {
        if(m_md==NULL)
        {
            QMessageBox::warning(this, tr("SilentEye Warning"),
                                 "No media has been selected, this is probably an internal error.\n please report it to developpers.");
            return false;
        }

        if(cryptoCheckBox->checkState()!=Qt::Unchecked)
        {
            if(!checkCryptoPassword())
            {
                QMessageBox::warning(this, tr("SilentEye Warning"),
                                     "Key for encryption process have to be valid ! (match and have more than 5 characters)");
                return false;
            }
        }

        if(m_checkSize && dataSizeRemaining() < 0)
        {
            QMessageBox::warning(this, tr("SilentEye Warning"),
                                 "Selected media doesn't have enough space to encode your message!");
            return false;
        }
        return true;
    }

    FormatModuleInterface* OptionDialog::currentFormatModule()
    {
        QString format = formatComboBox->currentText();

        FormatModuleInterface* module = NULL;
        if (m_md->type() == Media::IMAGE) {
            module = (FormatModuleInterface*) ModuleManager::get(ModuleManager::IMAGEFORMAT, format);
        } else if (m_md->type() == Media::AUDIO) {
            module = (FormatModuleInterface*) ModuleManager::get(ModuleManager::AUDIOFORMAT, format);
        } else if (m_md->type() == Media::VIDEO) {
            module = (FormatModuleInterface*) ModuleManager::get(ModuleManager::VIDEOFORMAT, format);
        }

        if(module == NULL) {
            m_logger->warning("Cannot find module for '" + format + "' format" );
        } else if(m_checkSize) {
            connect(module, SIGNAL(optionChanged()), this, SLOT(valueChanged()), Qt::UniqueConnection);
        }

        return module;
    }

    CryptoModuleInterface* OptionDialog::currentCryptoModule()
    {
        QString type = cryptoComboBox->currentText();
        return (CryptoModuleInterface*)ModuleManager::get(ModuleManager::CRYPTO, type);
    }

    Data::DataFormat OptionDialog::currentCharset()
    {
        Data::DataFormat charset = Data::F_UNDEF;
        switch(charsetComboBox->currentIndex())
        {
        case 0:
            charset = Data::UTF8;
            break;
        case 1:
            charset = Data::LATIN1;
            break;
        case 2:
            charset = Data::ASCII;
            break;
        }

        return charset;
    }

    void OptionDialog::formatChanged(const QString& imageFormat)
    {
        FormatModuleInterface* module = NULL;
        if (m_md->type() == Media::IMAGE)
        {
            module = (FormatModuleInterface*) ModuleManager::get(ModuleManager::IMAGEFORMAT, imageFormat);
        }
        else if (m_md->type() == Media::AUDIO)
        {
            module = (FormatModuleInterface*) ModuleManager::get(ModuleManager::AUDIOFORMAT, imageFormat);
        }
        else if (m_md->type() == Media::VIDEO)
        {
            module = (FormatModuleInterface*) ModuleManager::get(ModuleManager::VIDEOFORMAT, imageFormat);
        }

        if(module==NULL) {
            m_logger->warning("Cannot find module for '" + imageFormat + "' format" );
            return;
        }

        QWidget* w = currentFormatWidget(module);

        if(w!=NULL)
        {
            if(m_optionWidget!=NULL)
                m_optionWidget->setVisible(false);
            m_optionWidget = w;
            optionsVerticalLayout->insertWidget(0, m_optionWidget);
            m_optionWidget->setVisible(true);
        }
        else
            m_logger->warning("The widget for encoding format is NULL");

        valueChanged();
    }

    bool OptionDialog::checkCryptoPassword()
    {
        QPalette palette;
        bool return_value = false;

        if (cryptoPwd2LineEdit->text().length() >= 6
            && cryptoPwd1LineEdit->text() == cryptoPwd2LineEdit->text())
        {
            palette.setColor(QPalette::Base, QColor(179, 211, 155)); // green
            return_value = true;
        }
        else if (cryptoPwd2LineEdit->text().length() > 0 && cryptoPwd2LineEdit->text().length() <= 6)
            palette.setColor(QPalette::Base, QColor(255, 204, 204)); // red

        cryptoPwd1LineEdit->setPalette(palette);
        cryptoPwd2LineEdit->setPalette(palette);

        return return_value;
    }


    void OptionDialog::cryptoStatusChanged(int status)
    {
        if(status==Qt::Unchecked)
        {
            cryptoWidget->setVisible(false);
            cryptoPwd1LineEdit->setText("");
            cryptoPwd2LineEdit->setText("");
            checkCryptoPassword();
        }
        else
        {
            cryptoWidget->setVisible(true);        
        }
        updateDialogSize();
    }

    void OptionDialog::cryptoPwd1TextEdited(QString)
    {
        checkCryptoPassword();
    }

    void OptionDialog::cryptoPwd2TextEdited(QString)
    {
        checkCryptoPassword();
    }

    void OptionDialog::valueChanged()
    {
        if(m_checkSize)
        {
            //m_logger->debug("ValueChanged!");
            FormatModuleInterface* module = currentFormatModule();
            QPointer<Media> media;
            if(module!=NULL) {
                if (m_md->type() == Media::IMAGE) {
                    media = ((ImageModuleInterface*)module)->encodeImage(QPointer<Image>((Image*)m_md.data()), true);
                } else if (m_md->type() == Media::AUDIO) {
                    media = ((AudioModuleInterface*)module)->encodeAudio(QPointer<Audio>((Audio*)m_md.data()), true);
                } else if (m_md->type() == Media::VIDEO) {
                    media = ((VideoModuleInterface*)module)->encodeVideo(QPointer<Video>((Video*)m_md.data()), true);
                }
                dataSizeAvailableLabel->setText(QString::number(media->capacity() - 1)); // -1: "Data" format flag
            }

            int remaining = dataSizeRemaining(module, media); // this function delete media object
            
            QPalette palette;
            if (remaining < 0)
                palette.setColor(QPalette::WindowText, QColor(127, 0, 0));
            else
                palette.setColor(QPalette::WindowText, QColor(0, 127, 14));
            dataSizeRemainingLabel->setText(QString::number(remaining));
            dataSizeRemainingLabel->setPalette(palette);
        }
    }

    int OptionDialog::dataSizeRemaining(FormatModuleInterface* module, QPointer<Media> media) {
        long capacity = 0;
        if(module==NULL)
            module = currentFormatModule();

        if(module!=NULL) {

            if (media.isNull())
            {
                if (m_md->type() == Media::IMAGE) {
                    media = ((ImageModuleInterface*)module)->encodeImage(QPointer<Image>((Image*)m_md.data()), true);
                } else if (m_md->type() == Media::AUDIO) {
                    media = ((AudioModuleInterface*)module)->encodeAudio(QPointer<Audio>((Audio*)m_md.data()), true);
                } else if (m_md->type() == Media::VIDEO) {
                    media = ((VideoModuleInterface*)module)->encodeVideo(QPointer<Video>((Video*)m_md.data()), true);
                }
            }
            if (!media.isNull())
            {
                capacity = media->capacity();
                delete media;
            }
        }

        int size;
        if (m_filePath == "" || !QFile::exists(m_filePath))
        {
            int compressionLevel = 0;
            if (compressCheckBox->checkState()!=Qt::Unchecked)
            {
                compressionLevel = compressSpinBox->value();
            }
            EncodedData data(msgTextEdit->toPlainText(), currentCharset(),
                             compressionLevel);
            size = data.size();
        }
        else
        {
            int compressionLevel = 0;
            if (compressCheckBox->checkState()!=Qt::Unchecked)
            {
                compressionLevel = compressSpinBox->value();
            }
            QFile file(m_filePath);
            EncodedData data(file, compressionLevel);
            size = data.size();
        }

        return capacity-size;
    }

    void OptionDialog::loadFormats()
    {
        formatComboBox->clear();

        QMap<QString, ModuleInterface*> map;
        if (m_md->type() == Media::IMAGE) {
            map = ModuleManager::get(ModuleManager::IMAGEFORMAT);
            m_logger->debug(QString::number(map.size()) + " image module found");
            mediaFormatIcon->setPixmap(QPixmap(QString::fromUtf8(":/icons/img/imageformat.png")));
        } else if (m_md->type() == Media::AUDIO) {
            map = ModuleManager::get(ModuleManager::AUDIOFORMAT);
            m_logger->debug(QString::number(map.size()) + " audio module found");
            mediaFormatIcon->setPixmap(QPixmap(QString::fromUtf8(":/icons/img/play.png")));
        } else if (m_md->type() == Media::VIDEO) {
            map = ModuleManager::get(ModuleManager::VIDEOFORMAT);
            m_logger->debug(QString::number(map.size()) + " video module found");
        } else {
            m_logger->warning("Media type unknown!");
        }

        foreach (ModuleInterface* module, map)
        {
            formatComboBox->addItem(module->typeSupported());
            if (Controller::instance()->config.get("imageformat") == module->typeSupported()
                || Controller::instance()->config.get("audioformat") == module->typeSupported()
                || Controller::instance()->config.get("videoformat") == module->typeSupported())
            {
                formatComboBox->setCurrentIndex(formatComboBox->count()-1);
            }
        }
    }

    void OptionDialog::loadEncryptionTypes()
    {
        foreach(ModuleInterface* module,
                ModuleManager::get(ModuleManager::CRYPTO))
        {
            cryptoComboBox->addItem(module->typeSupported());
            if(Controller::instance()->config.get("encryption")==module->typeSupported())
            {
                cryptoComboBox->setCurrentIndex(cryptoComboBox->count()-1);
            }
        }
    }

    void OptionDialog::updateDialogSize()
    {
        int height = m_originalHeight;
        if (!cryptoWidget->isVisible()) {
            height -= cryptoWidget->minimumHeight();
            height -= mainGridLayout->verticalSpacing();
        }
        if (!dataWidget->isVisible()) {
            height -= dataWidget->minimumHeight();
            height -= mainGridLayout->verticalSpacing();
        }

        setMinimumHeight(height);
        resize(minimumWidth(), height);
    }

    void OptionDialog::compressionModeChange(int)
    {
        compressSpinBox->setEnabled(compressCheckBox->checkState()!=Qt::Unchecked);
        valueChanged();
    }

    void OptionDialog::setupDialog()
    {
        cryptoWidget->setVisible(false);

        if(ModuleManager::count(ModuleManager::CRYPTO)<=0)
            cryptoCheckBox->setVisible(false);

        if(ModuleManager::count(ModuleManager::IMAGEFORMAT)<=0)
        {
            imageFormatWidget->setEnabled(false);
            okButton->setEnabled(false);
        }

        if(Controller::instance()->config.get("charset")=="Latin1")
            charsetComboBox->setCurrentIndex(1);
        else if(Controller::instance()->config.get("charset")=="ASCII")
            charsetComboBox->setCurrentIndex(2);
        else
            charsetComboBox->setCurrentIndex(0);

        updateDialogSize();
    }

    void OptionDialog::connectSignals()
    {
        connect(okButton, SIGNAL(pressed()), this, SLOT(ok()));
        connect(this, SIGNAL(rejected()), this, SLOT(rejected()));
        connect(toolButton, SIGNAL(pressed()), this, SLOT(selectFolder()));
        connect(exportButton, SIGNAL(pressed()), this, SLOT(selectExportFolder()));
        connect(cryptoCheckBox, SIGNAL(stateChanged(int)),
                this, SLOT(cryptoStatusChanged(int)));
        connect(cryptoPwd1LineEdit, SIGNAL(textEdited(QString)),
                this, SLOT(cryptoPwd1TextEdited(QString)));
        connect(cryptoPwd2LineEdit, SIGNAL(textEdited(QString)),
                this, SLOT(cryptoPwd2TextEdited(QString)));
        connect(formatComboBox, SIGNAL(currentIndexChanged(const QString&)),
                this, SLOT(formatChanged(const QString&)));
        connect(msgTextEdit, SIGNAL(textChanged()),
                this, SLOT(valueChanged()));
        connect(compressCheckBox, SIGNAL(stateChanged(int)),
                this, SLOT(compressionModeChange(int)));
        connect(compressSpinBox, SIGNAL(valueChanged(int)),
                this, SLOT(valueChanged()));
        connect(fileRemoveButton, SIGNAL(pressed()), this, SLOT(removeFile()));
    }

    void OptionDialog::displayException(const QString& title,
                                        const SilentEyeException e){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle(title);
        msgBox.setText(e.message());
        msgBox.setDetailedText(e.details());
        msgBox.exec();
    }

}
