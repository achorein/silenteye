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

#ifndef SEFORMATWAV_H_
#define SEFORMATWAV_H_

#include <QtCore>
#include <audiomoduleinterface.h>
#include <logger.h>
#include <config.h>

#include "optionwidget.h"
#include "audiowav.h"

using namespace SilentEyeFramework;

//! SilentEye offical module for support of WAV audio
namespace SEFormatWAV {

    //! Official module for support of WAVE/PCM sound (using LSB)
    class SEFormatWav : public AudioModuleInterface
    {
        Q_OBJECT;
        Q_INTERFACES(SilentEyeFramework::AudioModuleInterface);

    private:
        QPointer<Logger> m_logger;
        OptionWidget* m_encodeWidget;
        OptionWidget* m_decodeWidget;
        QPointer<Config> m_config;

    public:
        SEFormatWav();
        ~SEFormatWav();

        QWidget* encodeWidget();
        QWidget* decodeWidget();
        bool isEncodeWidgetReady() const;
        bool isDecodeWidgetReady() const;

        QPointer<Audio> encodeAudio(QPointer<Audio> ,bool=false);
        QPointer<Audio> decodeAudio(QPointer<Audio>, bool=false);

        QString typeSupported() const;

        QString name() const;
        QString version() const;
        QString status();

    private:
        QPointer<Audio> updateAudio(QPointer<Audio>, OptionWidget*, bool);

    private slots:
        void optionHasChanged();

    signals:
        void optionChanged();
    };
}

#endif
