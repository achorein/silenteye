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

#include "propertydialog.h"
#include "modulemanager.h"

#include <imagemoduleinterface.h>
#include <audiomoduleinterface.h>
#include <videomoduleinterface.h>

namespace SilentEye {

    PropertyDialog::PropertyDialog(QWidget* parent)
        : QDialog(parent)
    {
        setObjectName("PropertyDialog");
        m_logger = new Logger(this);
        setupUi(this);
        m_md = NULL;

	connect(formatComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(formatChanged(const QString&)));
    }

    PropertyDialog::~PropertyDialog()
    {
      //        if (!m_logger.isNull())
      //        {
      //            delete m_logger;
      //        }
    }

    void PropertyDialog::setMedia(Media* md)
    {
        m_md = md;

        pathLabel->setText(m_md->filePath());

        if (m_md->type() == Media::IMAGE)
        {
            Image* img = (Image*) m_md;

            if (img->quality() > 0)
            {
                qualityLabel->setText(QString::number(img->quality()) + "%");
            }
            else
            {
                qualityLabel->setText("N/A");
            }

            dimensionGroupBox->setTitle("Dimension");
            dimensionLabel->setText( QString::number(img->width())
                                     + "x"
                                     + QString::number(img->height()) );

            formatLabel->setText("N/A");
            qualityGroupBox->setVisible(true);
        }
        else if (m_md->type() == Media::AUDIO)
        {
            Audio* audio = (Audio*) m_md;
            QString format = audio->format().codec().toUpper();
            if (audio->format().sampleType() == QAudioFormat::UnSignedInt) {
                format += " unsigned ";
            } else if (audio->format().sampleType() == QAudioFormat::SignedInt) {
                format += " signed ";
            }
            format += QString::number(audio->format().sampleSize()) + " bit";

            format += ", " + QString::number(audio->format().frequency()) + " Hz";
            format += ", " + QString::number(floor(audio->bitRate()/1000.0)) + " kbps";

            if (audio->format().channels() == 1) {
                format += ", mono";
            } else if (audio->format().channels() == 2) {
                format += ", stereo";
            } else {
                format += ", " + QString::number(audio->format().channels()) + " channels";
            }
            formatLabel->setText(format);

            dimensionGroupBox->setTitle("Duration");
            double sec = audio->duration();
            double val = sec/60.0;
            int min = floor(val);
            val = sec - (min*60);
            sec = floor(val);
            int mili = (val - sec)*1000;

            QString duration = QString::number(min).rightJustified(2, '0')
                               + ":" + QString::number(sec).rightJustified(2, '0')
                               + "." + QString::number(mili);

            dimensionLabel->setText(duration);
            qualityGroupBox->setVisible(false);
        }

        Media* media = NULL;
        QString capacityText = "";

        QMap<QString, ModuleInterface*> map;
        if (m_md->type() == Media::IMAGE) {
            map = ModuleManager::get(ModuleManager::IMAGEFORMAT);
        } else if (m_md->type() == Media::AUDIO) {
            map = ModuleManager::get(ModuleManager::AUDIOFORMAT);
        } else if (m_md->type() == Media::VIDEO) {
            map = ModuleManager::get(ModuleManager::VIDEOFORMAT);
        }

	formatComboBox->clear();
	m_capacityMap.clear();
        foreach(ModuleInterface* module, map)
        {
            if (m_md->type() == Media::IMAGE) {
                Image* img = ((ImageModuleInterface*) module)->encodeImage(QPointer<Image>((Image*)m_md), true);
                media = (Media*) img;
            } else if (m_md->type() == Media::AUDIO) {
                Audio* audio = ((AudioModuleInterface*) module)->encodeAudio(QPointer<Audio>((Audio*)m_md), true);
                media = (Media*) audio;
            } else if (m_md->type() == Media::VIDEO) {
                Video* vid = ((VideoModuleInterface*) module)->encodeVideo(QPointer<Video>((Video*)m_md), true);
                media = (Media*) vid;
            }

	    formatComboBox->addItem(module->typeSupported());
            // media = ((FormatModuleInterface*)module)->encodeMedia(m_md, true);
            m_capacityMap[module->typeSupported()] = media->capacity();
	    
            delete media;
        }
	formatChanged(formatComboBox->currentText());
    }

    void PropertyDialog::formatChanged(const QString& text) {
        long capacity = m_capacityMap[text];
        sizeLabelOctets->setText(QString::number(capacity)+" octects");
        sizeLabelKoctets->setText(QString::number(capacity/1024.0)+ " ko");
    }

}
