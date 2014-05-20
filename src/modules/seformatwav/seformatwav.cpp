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
//  along with SilentEye.  If not, see <http://www.gnu.org/licenses/>.

#include "seformatwav.h"

namespace SEFormatWAV {

    SEFormatWav::SEFormatWav()
    {
        this->setObjectName("SEFormatWav");
        m_logger = new Logger(this);

        m_encodeWidget = NULL;
        m_decodeWidget = NULL;

        #ifdef CONFPATH_USERHOME
            QString config_path = QDir::homePath()+"/.silenteye/";
        #elif defined(CONFPATH_VALUE)
            QString config_path = CONFPATH_VALUE + QString("/");
        #else
            QString config_path = qApp->applicationDirPath()+"/";
        #endif
        m_config = new Config(config_path, "seformatwav");
    }

    SEFormatWav::~SEFormatWav()
    {
        if (m_encodeWidget != NULL)
            delete m_encodeWidget;
        if (m_decodeWidget != NULL)
            delete m_decodeWidget;
        delete m_logger;
    }

    QString SEFormatWav::name() const
    {
        return QString("Silent Eye Audio Format WAVE");
    }

    QString SEFormatWav::version() const
    {
        return QString("1.2");
    }

    QString SEFormatWav::status()
    {
        return "OK|Configuration file " + m_config->fileAbsoluteName()
                + " " + (m_config->isLoaded() ? "loaded." : "not loaded.");
    }

    QWidget* SEFormatWav::encodeWidget()
    {
        if (m_encodeWidget == NULL)
        {
            m_encodeWidget = new OptionWidget();
            m_encodeWidget->setObjectName("EncodeWidget");
            if (!m_config.isNull() && m_config->isLoaded()) {
                m_logger->info("Load encode default options from configuration file: " + m_config->fileAbsoluteName());
                m_encodeWidget->setNbBits(m_config->getInt("nbBits"));
                m_encodeWidget->setNbChannelUsed(m_config->getInt("channels"));
                m_encodeWidget->setDistribution(m_config->get("distribution"));
                m_encodeWidget->setHeaderPosition(m_config->get("headerPosition"));
            }
            connect(m_encodeWidget, SIGNAL(optionHasChanged()), this, SLOT(optionHasChanged()));
        }
        return m_encodeWidget;
    }

    QWidget* SEFormatWav::decodeWidget()
    {
        if (m_decodeWidget == NULL)
        {
            m_decodeWidget = new OptionWidget();
            m_decodeWidget->setObjectName("DecodeWidget");
            if (!m_config.isNull() && m_config->isLoaded()) {
                m_logger->info("Load decode default options from configuration file: " + m_config->fileAbsoluteName());
                m_decodeWidget->setNbBits(m_config->getInt("nbBits"));
                m_decodeWidget->setNbChannelUsed(m_config->getInt("channels"));
                m_decodeWidget->setDistribution(m_config->get("distribution"));
                m_decodeWidget->setHeaderPosition(m_config->get("headerPosition"));
            }
        }
        return m_decodeWidget;
    }

    QPointer<Audio> SEFormatWav::encodeAudio(QPointer<Audio> audio, bool dontDuplicate)
    {
        return updateAudio(audio, (OptionWidget*) encodeWidget(), dontDuplicate);
    }

    QPointer<Audio> SEFormatWav::decodeAudio(QPointer<Audio> audio, bool dontDuplicate)
    {
        return updateAudio(audio, (OptionWidget*) decodeWidget(), dontDuplicate);
    }

    QPointer<Audio> SEFormatWav::updateAudio(QPointer<Audio> aud, OptionWidget* w, bool dontDuplicate)
    {
        QPointer<Audio> wav;
        if (dontDuplicate)
        {
            wav = new AudioWav(aud); // don't copy wave data
        }
        else
        {
            wav = new AudioWav(*aud); // duplicate wave into a new one
        }

        AudioWav* newAudio = ((AudioWav*)wav.data());

        newAudio->setNbBitsUsed(w->nbBits());
        newAudio->setNbChannelUsed(w->channels());
        newAudio->setDistribution(w->distribution());
        newAudio->setHeaderPosition(w->headerPosition());

        return wav;
    }


    QString SEFormatWav::typeSupported() const
    {
        return QString("WAVE");
    }

    bool SEFormatWav::isEncodeWidgetReady() const
    {
        return true;
    }

    bool SEFormatWav::isDecodeWidgetReady() const
    {
        return true;
    }

    void SEFormatWav::optionHasChanged()
    {
        emit optionChanged();
    }

    }

Q_EXPORT_PLUGIN2(seformatwav, SEFormatWAV::SEFormatWav)
