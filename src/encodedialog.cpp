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

#include "encodedialog.h"
#include "controller.h"

#include <imagemoduleinterface.h>
#include <audiomoduleinterface.h>
#include <videomoduleinterface.h>

namespace SilentEye {

    EncodeDialog::EncodeDialog(QWidget* parent)
        : OptionDialog(parent)
    {
        this->setObjectName("EncodeDialog");
        m_logger->setClassName("EncodeDialog");

        this->setWindowTitle("Encode message");
        this->setWindowIcon(QPixmap(QString::fromUtf8(":/icons/img/encode.png")));

        okButton->setText("Encode");
        iconLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/img/encode.png")));
        msgLabel->setText("Write a message or select a file to hide.");
        updateDialogSize();

        connect(fileButton, SIGNAL(pressed()), this, SLOT(selectFile()));
    }

    void EncodeDialog::showEvent(QShowEvent* event)
    {
        if(!event->spontaneous())
        {
            m_logger->debug("set default output dir: " + Controller::instance()->config.get("output"));
            destinationLineEdit->setText(Controller::instance()->config.get("output"));
            msgTextEdit->setText(m_message);
            cryptoPwd1LineEdit->setText("");
            cryptoPwd2LineEdit->setText("");
            checkCryptoPassword();
            formatChanged(formatComboBox->currentText());
            if(!m_md.isNull())
                this->setWindowTitle("Encode message : " + m_md->filePath());
            updateDialogSize();
            if (m_filePath == "")
                removeFile();
            else
                addFile(m_filePath);
        }
    }

    void EncodeDialog::dragEnterEvent(QDragEnterEvent *event)
    {
        if (event->mimeData()->hasUrls())
            event->acceptProposedAction();
        else
            m_logger->debug(event->mimeData()->formats().join(";"));
    }

    void EncodeDialog::dropEvent(QDropEvent *event)
    {
        if(event->mimeData()->hasUrls())
        {
            QList<QUrl> urls = event->mimeData()->urls();
            for(int i=0; i<urls.size(); i++)
            {
                if(urls.at(i).toLocalFile().length()>0)
                    addFile(urls.at(i).toLocalFile());
            }
        }
        else
        {
            QMessageBox::warning(this, tr("Warning"),
                                 "The dropped object doesn't have any url.");
        }

    }

    QWidget* EncodeDialog::currentFormatWidget(FormatModuleInterface* module)
    {
        if (module == NULL)
            return NULL;

        return module->encodeWidget();
    }

    void EncodeDialog::ok()
    {
        if(!isDialogReady())
            return;

        /*
         * CHECK widgets are ready
         */
        if(msgTextEdit->toPlainText().length()==0 && m_filePath == "")
        {
            QMessageBox::critical(this, tr("SilentEye Warning"),
                                  "You must provide a message or select a file to hide.");
            return;
        } else if (m_filePath != "" && !QFile::exists(m_filePath)) {
            QMessageBox::critical(this, tr("SilentEye Warning"),
                                  "Selected file doesn't exist!");
            return;
        }

        QString output = destinationLineEdit->text();
        if(!QFile::exists(output))
        {
            QMessageBox::warning(this, tr("SilentEye Warning"),
                                 "Destination folder doesn't exists!");
            return;
        }

        if(!currentFormatModule()->isEncodeWidgetReady())
        {
            setCursor(Qt::ArrowCursor);
            QMessageBox::warning(this, tr("SilentEye Warning"),
                                 QString("Wrong options for the current encoding format.\n")+
                                 "Please check your options values before launching the encoding process.");
            return;
        }

        // NOW PROCESS BEGIN

        setCursor(Qt::WaitCursor);

        if (defaultCheckBox->checkState()!=Qt::Unchecked)
        {
            Controller::instance()->config.set("output", output);
            Controller::instance()->config.save();
        }

        CryptoModuleInterface* module;
        try
        {
            // don't compress yet if crypto enabled
            bool compress = compressCheckBox->checkState()!=Qt::Unchecked && cryptoCheckBox->checkState()==Qt::Unchecked;
            QPointer<EncodedData> data;
            if (m_filePath != "" && QFile::exists(m_filePath))
            {
                m_logger->debug("File to hide: " + m_filePath);
                QFile file(m_filePath);
                data = new EncodedData(file, compress);
            }
            else
            {
                m_logger->debug("Message to hide: " + msgTextEdit->toPlainText());
                data = new EncodedData(msgTextEdit->toPlainText(), currentCharset(), compress);
            }

            if(cryptoCheckBox->checkState()!=Qt::Unchecked)
            {
                module = currentCryptoModule();
                if(module!=NULL)
                {
                    data = module->encode(cryptoPwd2LineEdit->text(), data);
                    data->setCompressed(compressCheckBox->checkState()!=Qt::Unchecked, true);
                }
                else
                    m_logger->warning("Invalid encryption type");
            }

            if (m_md->type() == Media::IMAGE) {
                Image* img = ((ImageModuleInterface*) currentFormatModule())->encodeImage(QPointer<Image>((Image*)m_md.data()), false);
                m_md = QPointer<Media>(img);
            } else if (m_md->type() == Media::AUDIO) {
                m_md = ((AudioModuleInterface*) currentFormatModule())->encodeAudio(QPointer<Audio>((Audio*)m_md.data()), false);
            } else if (m_md->type() == Media::VIDEO) {
                m_md = ((VideoModuleInterface*) currentFormatModule())->encodeVideo(QPointer<Video>((Video*)m_md.data()), false);
            }

            if (!output.endsWith('/'))
            {
                output += "/";
            }
            m_logger->debug(output+m_md->shortName() + " == " + m_md->filePath());
            if (m_md->filePath().compare(output+m_md->shortName(), Qt::CaseInsensitive) == 0)
            {
                setCursor(Qt::ArrowCursor);
                QMessageBox::warning(this, tr("SilentEye Warning"),
                                     "Cannot save new file into source directory, please select an other destination!");
                return;
            }
            else if (QFile::exists(output+m_md->shortName()))
            {
                setCursor(Qt::ArrowCursor);
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Question);
                msgBox.setText("Destination file already exists !");
                msgBox.setDetailedText(output+m_md->shortName());
                msgBox.setInformativeText("Do you really want to override it ?");
                msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Cancel);
                if(msgBox.exec()==QMessageBox::Cancel)
                    return;
            }

            m_md->setEncodedData(data);

            QString errorMsg;
            if( m_md->saveToDir(output) )
            {
                setCursor(Qt::ArrowCursor);
                emit encodedImage(m_md->filePath());
                this->accept();
            }
            else {
                errorMsg += "- An error occured during the encoding process.\n";
                errorMsg += "- Cannot save the output file : "+m_md->filePath()+".\n";
            }

            if(errorMsg.length() > 0)
                QMessageBox::critical(this, tr("SilentEye Error"), errorMsg);
        }
        catch(ModuleException e)
        {
            if (module != NULL)
                displayException(module->name(), e);
            else
                displayException("Module unknown", e);
        }
        catch(SilentEyeException e)
        {
            displayException(module->name(), e);
        }

        setCursor(Qt::ArrowCursor);
    }

}
