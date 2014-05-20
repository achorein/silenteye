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

#ifndef VIDEO_H_
#define VIDEO_H_

#include "media.h"

#include <QString>

namespace SilentEyeFramework {

    //! Generic representation of video which can load and hide specific data according to it's format
    class Video : public Media
    {
    protected:
        //! track length
        int m_length;

    public:
        Video();
        ~Video();
        Video(const QString& filepath);
        Video(const Video&);
        Video(Video*);
    };

}

#endif
