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

#include "preferencedialog.h"
#include "modulemanager.h"
#include "controller.h"

namespace SilentEye {

    PreferenceDialog::PreferenceDialog(QWidget* parent) 
        : QDialog(parent)
    {
        setupUi(this);

        unsigned short int nbImageFormat = 0;
        unsigned short int nbAudioFormat = 0;
        unsigned short int nbCrypto = 0;

        if(ModuleManager::count()>0)
        {
            moduleListWidget->clear();

            QList<ModuleInterface*> moduleList = ModuleManager::getList();
            for(int i=0; i<moduleList.size(); i++)
            {
                ModuleInterface* module = moduleList.at(i);

                QListWidgetItem* item = new QListWidgetItem(module->name()+" ("+module->version()+")", moduleListWidget);

                if(!module->status().startsWith("ok", Qt::CaseInsensitive))
                {
                    QBrush brush(QColor(255, 225, 225));
                    item->setBackground(brush);
                }
                item->setToolTip(module->status().section("|", 1, 1));

                ModuleManager::Category cat = ModuleManager::getCategory(module);
                if (cat == ModuleManager::IMAGEFORMAT)
                {
                    item->setIcon(QPixmap(":/icons/img/imageformat.png"));
                    imageFormatComboBox->addItem(module->typeSupported());
                    m_imageFormatMap[module->typeSupported()] = nbImageFormat++;
                }
                else if(cat == ModuleManager::AUDIOFORMAT)
                {
                    item->setIcon(QPixmap(":/icons/img/play.png"));
                    audioFormatComboBox->addItem(module->typeSupported());
                    m_audioFormatMap[module->typeSupported()] = nbAudioFormat++;
                }
                else if(cat == ModuleManager::CRYPTO)
                {
                    item->setIcon(QPixmap(":/icons/img/crypto.png"));
                    cryptoComboBox->addItem(module->typeSupported());
                    m_cryptoMap[module->typeSupported()] = nbCrypto++;
                }
            }
        }

        this->setConfig();

        connect(this, SIGNAL(accepted()), this, SLOT(accepted()));
        connect(this, SIGNAL(rejected()), this, SLOT(rejected()));
        connect(outputToolButton, SIGNAL(pressed()), this, SLOT(selectFile()));
        connect(proxyCheckBox, SIGNAL(stateChanged(int)), this, SLOT(useHttpProxy(int)));

        proxyLoginLabel->setVisible(false);
        proxyLoginLineEdit->setVisible(false);
        proxyPasswordLabel->setVisible(false);
        proxyPasswordLineEdit->setVisible(false);
    }

    void PreferenceDialog::setConfig()
    {
        outputLineEdit->setText(Controller::instance()->config.get("output"));

        // IMAGE
        if(m_imageFormatMap.contains(Controller::instance()->config.get("imageformat")))
        {
            imageFormatComboBox->setCurrentIndex(
                    m_imageFormatMap[Controller::instance()->config.get("imageformat")]
                    );
        }
        else
            imageFormatComboBox->setCurrentIndex(0);

        // AUDIO
        if(m_imageFormatMap.contains(Controller::instance()->config.get("audioformat")))
        {
            audioFormatComboBox->setCurrentIndex(
                    m_audioFormatMap[Controller::instance()->config.get("audioformat")]
                    );
        }
        else
            audioFormatComboBox->setCurrentIndex(0);

        // CRYPTO
        if(m_cryptoMap.contains(Controller::instance()->config.get("encryption")))
        {
            cryptoComboBox->setCurrentIndex(
                    m_cryptoMap[Controller::instance()->config.get("encryption")]
                    );
        }
        else
            cryptoComboBox->setCurrentIndex(0);

        // CHARSET
        if(Controller::instance()->config.get("charset")=="Latin1")
            charsetComboBox->setCurrentIndex(1);
        else if(Controller::instance()->config.get("charset")=="ASCII")
            charsetComboBox->setCurrentIndex(2);
        else
            charsetComboBox->setCurrentIndex(0);

        // COMPRESSION
        if(Controller::instance()->config.isEmpty("compressdata") || Controller::instance()->config.getBool("compressdata"))
            compressCheckBox->setChecked(true);
        else
            compressCheckBox->setChecked(false);

        // PROXY
        bool proxy = Controller::instance()->config.getBool("proxyenable");
        if (proxy)
        {
            proxyCheckBox->setChecked(true);
            proxyHostLineEdit->setEnabled(true);
            proxyPortSpinBox->setEnabled(true);
        }
        proxyHostLineEdit->setText(Controller::instance()->config.get("proxyhost"));
        proxyPortSpinBox->setValue(Controller::instance()->config.getInt("proxyport"));
    }

    void PreferenceDialog::accepted()
    {
        Controller::instance()->config.set("output", outputLineEdit->text());
        Controller::instance()->config.set("imageformat", imageFormatComboBox->currentText());
        Controller::instance()->config.set("audioformat", audioFormatComboBox->currentText());
        Controller::instance()->config.set("compressdata", compressCheckBox->isChecked()?"1":"0");
        Controller::instance()->config.set("charset", charsetComboBox->currentText());
        Controller::instance()->config.set("encryption", cryptoComboBox->currentText());
        Controller::instance()->config.set("proxyenable", proxyCheckBox->isChecked()?"1":"0");
        Controller::instance()->config.set("proxyhost", proxyHostLineEdit->text());
        Controller::instance()->config.set("proxyport", QString::number(proxyPortSpinBox->value()));
        Controller::instance()->config.save();
        Controller::instance()->updateProxySettings();
    }

    void PreferenceDialog::rejected()
    {
        setConfig();
    }

    void PreferenceDialog::selectFile()
    {
        setCursor(Qt::WaitCursor);
        QFileDialog dialog(this, tr("Select default output dir"));
        dialog.setViewMode(QFileDialog::List);
        dialog.setFileMode(QFileDialog::DirectoryOnly);
        dialog.setDirectory(QDir::homePath());
        if(dialog.exec())
        {
            QStringList fileNames = dialog.selectedFiles();
            if(fileNames.size()>0)
            {
                if(QFile::exists(fileNames.at(0)))
                    outputLineEdit->setText(fileNames.at(0));
                else
                    QMessageBox::warning(this, tr("Warning"),
                                         fileNames.at(0)+"\n"+
                                         "The folder selected doesn't exists.\n"+
                                         "Please select an other destination !");
            }
        }
        setCursor(Qt::ArrowCursor);
    }


    void PreferenceDialog::useHttpProxy(int value)
    {
        bool enable = (value != Qt::Unchecked);
        proxyHostLineEdit->setEnabled(enable);
        proxyPortSpinBox->setEnabled(enable);
    }
}

