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
        return "OK|";
    }

    ModuleWidgetInterface* SEFormatWav::encodeWidget()
    {
        if (m_encodeWidget == NULL)
        {
            m_encodeWidget = new OptionWidget();
            m_encodeWidget->setObjectName("EncodeWidget");
            connect(m_encodeWidget, SIGNAL(optionHasChanged()), this, SLOT(optionHasChanged()));
        }
        return m_encodeWidget;
    }

    ModuleWidgetInterface* SEFormatWav::decodeWidget()
    {
        if (m_decodeWidget == NULL)
        {
            m_decodeWidget = new OptionWidget();
            m_decodeWidget->setObjectName("DecodeWidget");
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

    void SEFormatWav::optionHasChanged()
    {
        emit optionChanged();
    }

    }

Q_EXPORT_PLUGIN2(seformatwav, SEFormatWAV::SEFormatWav)
