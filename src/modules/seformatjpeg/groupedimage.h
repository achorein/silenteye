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

#ifndef GROUPEDIMAGE_H_
#define GROUPEDIMAGE_H_

#define PIXEL_GROUP_SIZE 8

#include <QObject>
#include <QPointer>
#include <QImage>
#include <QVector>

#include <logger.h>
#include "ycbcr.h"

using namespace SilentEyeFramework;

namespace SEFormatJPEG {

    //! The pixel group wich represent a block. (8x8 in the Jpeg format)
    /*! Each pixel in the pixel group is encode using the YCbCr format.
        In this class, pixel groups aren't considered as two dimention arrays. Blocs are managed in a one dimention array
    */
    class PixelGroup : public QObject{
    private:
        //! Logger
        QPointer<Logger> m_logger;
        
        //! Pixels contained in the pixel group
        QVector< QPointer<YCbCr> > m_pixels;
        //! Mean intensity value of the Y component pixels
        float m_miv;
        
    public:
    	PixelGroup(QObject* parent=0);
        ~PixelGroup();

        //! Get pixel at specified position
        /*!
           \param x horizontal position
           \param y vertical position
        */
        QPointer<YCbCr> pixel(quint8, quint8);
        //! Change pixel at specified position
        /*!
           \param x horizontal position
           \param y vertical position
        */
        void setPixel(quint8, quint8, QPointer<YCbCr>);
        
        float miv() const;
        //! Mean intensity value of the Y component pixels
        void updateMivTo(float);
        
        QString toString() const;

        static quint8 width() { return PIXEL_GROUP_SIZE; }
        static quint8 height() { return PIXEL_GROUP_SIZE; }
        
    private:
        //! Compute the Mean Intensity Value (the average of the Y component of the YCbCr format) of the block and store it in the MIV attribute.
        void computeMIV();
        //! Alter pixels of the block making the Mean Intensity Value to the destMIV
        void updatePixelMiv(QPointer<YCbCr>, double);
    };

    //! This class reprensents the image where pixels are grouped in square blocs
    class GroupedImage : public QObject{
    private:
        //! Logger
        QPointer<Logger> m_logger;

        //! Number of pixel groups in the width of the image
        quint16 m_width;
        //! Number of pixel groups in the height of the image
        quint16 m_height;
        //! The initial width of the image, in pixel
        quint16 m_initialWidth;
        //! The initial height of the image, in pixel
        quint16 m_initialHeight;
        //! The initial format of the image (QImage::Format)
        QImage::Format m_initialFormat;

        //! The image is stored in this two dimention array of PixelGroup
        QVector< QPointer<PixelGroup> > m_pg;
    public:
        GroupedImage(QImage&, quint8, QObject* parent=0);
        ~GroupedImage();

        quint16 width();
        quint16 height();
        quint16 initialWidth();
        quint16 initialHeight();

        //! Get pixel group at specified position
        /*!
           \param x horizontal position
           \param y vertical position
        */
        QPointer<PixelGroup> pixelGroup(quint16, quint16);

        //! Convert into Image
        QImage* toImage();
        //! Compact image (change luminance)
        static void compactImage(QImage&, quint8);

    private:
        void compacteAndGroupImage(QImage&, quint8);
        //! compact luminance (between +K et 255-K)
        static void compactPixel(QPointer<YCbCr>, quint8);
    };

}

#endif
