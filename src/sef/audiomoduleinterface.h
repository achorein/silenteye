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
//  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#ifndef AUDIOMODULEINTERFACE_H_
#define AUDIOMODULEINTERFACE_H_

#include "formatmoduleinterface.h"
#include "audio.h"

namespace SilentEyeFramework {

    //! Common interface for audio format plug-ins
    class AudioModuleInterface : public FormatModuleInterface
    {
    public:
        virtual QPointer<Audio> encodeAudio(QPointer<Audio>, bool=false) = 0;
        virtual QPointer<Audio> decodeAudio(QPointer<Audio>, bool=false) = 0;
    };

}

Q_DECLARE_INTERFACE(SilentEyeFramework::AudioModuleInterface,
                    "org.silenteye.sef.AudioModuleInterface/1.0");

#endif
