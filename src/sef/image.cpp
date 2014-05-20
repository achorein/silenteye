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

#include "image.h"

namespace SilentEyeFramework {

    Image::Image()
        : QPixmap(), Media()
    {
        m_type = IMAGE;
        setObjectName("Image");
    }

    Image::Image(const QString& filePath)
        : QPixmap(filePath), Media(filePath)
    {
        setObjectName("Image");
        m_type = IMAGE;
        m_width = width();
        m_height = height();
    }

    Image::Image(const QPixmap& pixmap, QString filePath)
        : QPixmap(pixmap), Media(filePath)
    {
        setObjectName("Image");
        m_type = IMAGE;
        m_width = width();
        m_height = height();
    }

    Image::Image(const Image& img)
        : QPixmap(img), Media(img)
    {
        m_type = IMAGE;
        m_width = img.imgWidth();
        m_height = img.imgHeight();
        setObjectName("Image");
    }

    Image::Image(Image* img)
        : Media(img)
    {
        m_type = IMAGE;
        setObjectName("Image");
        m_width = img->imgWidth();
        m_height = img->imgHeight();
    }

    Image::~Image()
    {
        // NOTHING TO DO
    }

    int Image::imgWidth() const
    {
        return m_width;
    }

    int Image::imgHeight() const
    {
        return m_height;
    }

    int Image::quality() const
    {
        return m_quality;
    }
}
