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

#ifndef IMAGE_H_
#define IMAGE_H_

#include "media.h"

#include <QPixmap>
#include <QPointer>
#include <QString>
#include <QDir>
#include "encodeddata.h"

namespace SilentEyeFramework {

    //! Generic representation of image which can load and hide specific data according to it's format
    class Image : public QPixmap, public Media
    {
    protected:
        //! Image width
        int m_width;
        //! Image height
        int m_height;
        //! Quality (in pourcent)
        int m_quality;

    public:
        Image();
        ~Image();
        Image(const QString& filepath);
        Image(const QPixmap& pixmap, QString filePath="Pixmap");
        Image(const Image& img);
        Image(Image* img);

        int imgWidth() const;
        int imgHeight() const;
        int quality() const;
    };

}

#endif
