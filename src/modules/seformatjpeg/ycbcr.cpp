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

#include "ycbcr.h"

#include "math.h"

namespace SEFormatJPEG {

    YCbCr::YCbCr(const float y, const float cb, const float cr, QObject* parent) : QObject(parent)
    {
        m_y = y;
        m_cb = cb;
        m_cr = cr;
    }

    YCbCr::YCbCr(const QRgb& rgb, QObject* parent) : QObject(parent)
    {        
        int r = qRed(rgb);
        int g = qGreen(rgb);
        int b = qBlue(rgb);

        m_y = (kr * r) + ((1 - kr - kb) * g) + (kb * b);
        m_cb = 128 - (0.1687 * r) - (0.3313 * g) + (0.5 * b);
        m_cr = 128 + (0.5 * r) - (0.4187 * g) - (0.0813 * b);
    }

    YCbCr::~YCbCr()
    {
        // NOTHING TO DO
    }

    float YCbCr::y() const
    {
        return m_y;
    }
    
    void YCbCr::setY(float value)
    {
    	m_y = value;
    }

    float YCbCr::cb() const
    {
        return m_cb;
    }

    void YCbCr::setCb(float value)
    {
    	m_cb = value;
    }
    
    float YCbCr::cr() const
    {
        return m_cr;
    }

    void YCbCr::setCr(float value)
    {
    	m_cr = value;
    }
    
    QRgb YCbCr::toRGB()
    {
        int red = round(m_y + 1.402 * (m_cr - 128));
        int green = round(m_y - (0.34414 * (m_cb - 128)) - (0.71414 * (m_cr-128)));
        int blue = round(m_y + (1.772 * (m_cb - 128)));

        /*int red = (int)(2 * (1 - kr) * m_cr + m_y);
        int green = (int)(m_y - 2 * ((1 - kr) * kr * m_cr + (1 - kb) * kb * m_cb) / (1 - kr - kb));
        int blue = (int)(2 * (1 - kb) * m_cb + m_y);*/

        // After the compacting process some computed value could be invalid.
        if (red > 255) red = 255;
        else if (red < 0) red = 0;
        if (green > 255) green = 255;
        else if (green < 0) green = 0;
        if (blue > 255) blue = 255;
        else if (blue < 0) blue = 0;

        return qRgb(red, green, blue);
    }

    QString YCbCr::toString()
    {
        QString str = "Y:" + QString::number(m_y) + ",";
        str += "Cb:" + QString::number(m_cb) + ",";
        str += "Cr:" + QString::number(m_cr);
        return str;
    }

    bool YCbCr::operator==(const YCbCr& color)
    {
        return round(color.y()) == round(m_y);
    }

}
