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

#include "decodedialog.h"
#include "controller.h"

#include <imagemoduleinterface.h>
#include <audiomoduleinterface.h>
#include <videomoduleinterface.h>

namespace SilentEye {

    DecodeDialog::DecodeDialog(QWidget* parent)
        : OptionDialog(parent)
    {
        this->setObjectName("DecodeDialog");
        m_logger->setClassName("DecodeDialog");
        m_checkSize = false;

        this->setWindowTitle("Decode message");
        this->setWindowIcon(QPixmap(QString::fromUtf8(":/icons/img/decode.png")));

        exportLabel->setText("Load conf:");
        okButton->setText("Decode");
        iconLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/img/decode.png")));
        msgLabel->setText("Decoded message");
        fileNameLabel->setText("");
        fileButton->setEnabled(false);
        fileRemoveButton->setVisible(false);

        outputWidget->setVisible(false);
        dataSizeWidget->setVisible(false);
        compressCheckBox->setText("Compressed data");
        compressSpinBox->setVisible(false);
        cryptoCheckBox->setText("Encrypted data");
        msgTextEdit->setReadOnly(true);
        dataWidget->setVisible(false);
        orLabel->setVisible(false);
        updateDialogSize();

        connect(fileButton, SIGNAL(clicked(bool)), this, SLOT(selectFile()));
        //connect(fileButton, SIGNAL(released()), this, SLOT(releaseFileButton()));
    }

    DecodeDialog::~DecodeDialog()
    {
    }

    void DecodeDialog::showEvent(QShowEvent* event)
    {
        if(!event->spontaneous())
        {
            msgTextEdit->setText("");
            fileButton->setEnabled(false);
            cryptoPwd1LineEdit->setText("");
            cryptoPwd2LineEdit->setText("");
            checkCryptoPassword();
            formatChanged(formatComboBox->currentText());
            removeFile();
            if(!m_md.isNull())
                this->setWindowTitle("Decode message: " + m_md->filePath());
            dataWidget->setVisible(false);
            updateDialogSize();
        }
    }

    QWidget* DecodeDialog::currentFormatWidget(FormatModuleInterface* module)
    {
        if (module == NULL)
            return NULL;

        return module->decodeWidget();
    }

    void DecodeDialog::ok()
    {
        if(!isDialogReady())
            return;
        msgTextEdit->setText("");
        removeFile();

        FormatModuleInterface* formatModule = currentFormatModule();
        if(formatModule == NULL || !formatModule->decodeWidget()->isReady())
        {
            QMessageBox::warning(this, tr("SilentEye Warning"),
                                 QString("Wrong options for the current encoding format.\n")+
                                 "Please check your options values before launching the decoding process.");
            return;
        }

        setCursor(Qt::WaitCursor);

        CryptoModuleInterface* module = NULL;
        try
        {
            // downcast to daughter class
            if (m_md->type() == Media::IMAGE) {
                Image* img = ((ImageModuleInterface*) currentFormatModule())->decodeImage(QPointer<Image>((Image*)m_md.data()));
                m_md = QPointer<Media>(img);
            } else if (m_md->type() == Media::AUDIO) {
                m_md = ((AudioModuleInterface*) currentFormatModule())->decodeAudio(QPointer<Audio>((Audio*)m_md.data()));
            } else if (m_md->type() == Media::VIDEO) {
                m_md = ((VideoModuleInterface*) currentFormatModule())->decodeVideo(QPointer<Video>((Video*)m_md.data()));
            }

            if(m_md->loadData())
            {
                QPointer<EncodedData> data = m_md->encodedData();
                data->setCompressed(compressCheckBox->checkState()!=Qt::Unchecked);

                if(cryptoCheckBox->checkState()!=Qt::Unchecked)
                {
                    module = currentCryptoModule();
                    if(module!=NULL)
                    {
                        QPointer<EncodedData> dataCrypto = module->decode(cryptoPwd2LineEdit->text(), data);
                        delete data;
                        data = dataCrypto;
                        m_md->setEncodedData(data);
                    }
                    else
                        m_logger->warning("Invalid encryption type");
                }

                switch(data->format())
                {
                case Data::FILE:
                    msgLabel->setText("Decoded file");
                    m_filePath = data->toData()->name();
                    fileNameLabel->setText(m_filePath);
                    msgTextEdit->setVisible(false);
                    fileWidget->setVisible(true);
                    fileButton->setEnabled(true);
                    dataWidget->setVisible(true);
                    updateDialogSize();
                    break;
                case Data::ASCII:
                case Data::UTF8:
                case Data::LATIN1:
                    msgLabel->setText("Decoded message");
                    msgTextEdit->setText(data->toString());
                    msgTextEdit->setVisible(true);
                    fileWidget->setVisible(false);
                    dataWidget->setVisible(true);
                    updateDialogSize();
                    break;
                default:
                    QMessageBox::warning(this, tr("SilentEye Warning"),
                                         "This media don't seem to have an hidden message.");
                }

            } else {
                QMessageBox::warning(this, tr("SilentEye Warning"),
                                     "This media don't seem to have an hidden message.");
            }

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
            displayException("SilentEye Warning", e);
        }
        setCursor(Qt::ArrowCursor);
    }

    void DecodeDialog::releaseFileButton()
    {
        if (!fileButton->isDown())
        {
            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;

            mimeData->setData("application/octet-stream", m_filePath.toUtf8());
            drag->setMimeData(mimeData);

            drag->exec(Qt::MoveAction);
        }
    }

}
