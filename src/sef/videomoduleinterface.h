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

#ifndef VIDEOMODULEINTERFACE_H_
#define VIDEOMODULEINTERFACE_H_

#include "formatmoduleinterface.h"
#include "video.h"

namespace SilentEyeFramework {

    //! Common interface for video format plug-ins
    class VideoModuleInterface : public FormatModuleInterface
    {
    public:
        virtual QPointer<Video> encodeVideo(QPointer<Video>, bool=false) = 0;
        virtual QPointer<Video> decodeVideo(QPointer<Video>, bool=false) = 0;
    };

}

Q_DECLARE_INTERFACE(SilentEyeFramework::VideoModuleInterface,
                    "org.silenteye.sef.VideoModuleInterface/1.0");

#endif
