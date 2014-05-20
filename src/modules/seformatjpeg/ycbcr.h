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

#ifndef COLORYCBCR_H_
#define COLORYCBCR_H_

#include <QObject>
#include <QRgb>
#include <QString>

namespace SEFormatJPEG {

    //! Manage the YCbCr value for a color corresponding to a pixel (Y: luminance, Cb: blue difference, Cr: red difference)
    class YCbCr : public QObject
    {
    private:
        //! luminance
        float m_y;
        //! blue difference
        float m_cb;
        //! red difference
        float m_cr;

        //! Mathematic value for conversion
        static const float kb = 0.114;
        //! Mathematic value for conversion
        static const float kr = 0.299;

    public:
        //! Create an YCbCr pixel from YCbCr
        YCbCr(const float, const float, const float, QObject* parent=0);
        //! Create an YCbCr pixel from RGB
        YCbCr(const QRgb&, QObject* parent=0);
        ~YCbCr();

        float y() const;
        void setY(float);
        
        float cb() const;
        void setCb(float);
        
        float cr() const;
        void setCr(float);

        //! Convert the YCbCr value into RGB Color
        QRgb toRGB();
        
        QString toString();

        bool operator==(const YCbCr&);
    };
}

#endif
