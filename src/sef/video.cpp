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

#include "video.h"

namespace SilentEyeFramework {

    Video::Video()
        : Media()
    {
        setObjectName("Video");
        m_type = VIDEO;
    }

    Video::Video(const QString& filePath)
        : Media(filePath)
    {
        setObjectName("Video");
        m_type = VIDEO;
    }

    Video::Video(const Video& aud)
        : Media(aud)
    {
        setObjectName("Video");
        m_type = VIDEO;
    }

    Video::Video(Video* aud)
        : Media(aud)
    {
        setObjectName("Video");
        m_type = VIDEO;
    }

    Video::~Video()
    {
        // NOTHING TO DO
    }

}
