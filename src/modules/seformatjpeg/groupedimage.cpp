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

#include "groupedimage.h"
#include <moduleexception.h>
#include <QDebug>

#include <math.h>
//#include <algorithm>

using namespace SilentEyeFramework;

namespace SEFormatJPEG {

    PixelGroup::PixelGroup(QObject* parent) : QObject(parent)
    {
    	setObjectName("PixelGroup");
        m_logger = new Logger(this);

    	m_pixels.resize(PIXEL_GROUP_SIZE*PIXEL_GROUP_SIZE);
    	m_miv = -1;
    }

    PixelGroup::~PixelGroup() {
    	if (!m_logger.isNull())
        {
            delete m_logger;
        }
        
        /*for(int i=0; i< m_pixels.size(); i++)
        {
            if (!m_pixels[i].isNull())
            {
                delete m_pixels[i];
            }
        }*/
    }

    QPointer<YCbCr> PixelGroup::pixel(quint8 x, quint8 y)
    {
    	int index = (y * PIXEL_GROUP_SIZE) + x;
        
    	if (index >= PIXEL_GROUP_SIZE*PIXEL_GROUP_SIZE)
        {
            m_logger->warning("cannot get pixel: position is out of range (x:" + QString::number(x)
                              + ",y:"+ QString::number(y) + ")");
            return QPointer<YCbCr>();
        }
        else 
        {
            return m_pixels[index];
        }
    }
    
    void PixelGroup::setPixel(quint8 x, quint8 y, QPointer<YCbCr> pixel)
    {
    	int index = (y * PIXEL_GROUP_SIZE) + x;
        
        if (index >= PIXEL_GROUP_SIZE*PIXEL_GROUP_SIZE)
        {
            m_logger->warning("cannot set pixel: position is out of range (x:"
                              + QString::number(x) + ",y:"+ QString::number(y) + ")");
        }
        else
        {
            m_pixels[index] = pixel;
            computeMIV();
        }
    }
    
    float PixelGroup::miv() const
    {
        return m_miv;
    }

    void PixelGroup::updateMivTo(float destMiv)
    {
        if (destMiv == m_miv)
        {
            return;
        }
        else if (destMiv < 0 || destMiv > 255)
        {
            m_logger->warning("Ignore MIV update> dest: " + QString::number(destMiv) + ",current: " + QString::number(m_miv));
            return;
        }

        qint8 tries = 0;
        double distance = destMiv - m_miv;
        do
        {
            for (int y=0; y<height(); y++)
            {
                for (int x=0; x<width(); x++)
                {
                    int qx = (x < width()/2.0) ? x+1 : width() - x;
                    int qy = (y < height()/2.0) ? y+1 : height() - y;
                    int q = std::min(qx, qy);
                    updatePixelMiv(pixel(x, y), (q/(width()/4.0)) * distance);
                }
            }

            computeMIV();
            tries++;

            distance = destMiv - m_miv;

        } while (abs(distance) > 0.1 && tries < 4);

        if (abs(m_miv - destMiv) > 1)
        {
            m_logger->warning("MIV destination not reached> dest: " + QString::number(destMiv)
                              + ",new: " + QString::number(m_miv));
        }
    }

    void PixelGroup::updatePixelMiv(QPointer<YCbCr> p, double distance)
    {
        if (!p.isNull() && p->y() != -1)
        {
            if (p->y() + distance < 0)
            {
                p->setY(0);
            }
            else if (p->y() + distance > 255)
            {
                p->setY(255);
            }
            else
            {
                p->setY(p->y() + distance);
            }
        }

    }

    void PixelGroup::computeMIV()
    {
        int cpt = 0;
        double sum = 0;

        for (int i=0; i<PIXEL_GROUP_SIZE*PIXEL_GROUP_SIZE; i++)
        {
            QPointer<YCbCr> p = m_pixels[i];
            if (!p.isNull() && p->y() >=0 && p->y() <= 255)
            {
                sum = sum + p->y();
                cpt = cpt + 1;
            }
        }

        if (cpt > 0)
        {
            m_miv = sum / cpt;
            //m_miv = round(sum / cpt);
        }
        else
        {
            m_miv = -1;
        }
    }
    
    QString PixelGroup::toString() const
    {
        QString str = "";
        for (int i=0; i<PIXEL_GROUP_SIZE*PIXEL_GROUP_SIZE; i++)
        {
            str += "[" + QString::number(i) + "|";
            QPointer<YCbCr> p = m_pixels[i];
            if (!p.isNull())
            {
                str += p->toString();
            } else {
                str += "null";
            }
            str += "]";
        }
        return str + "(" + QString::number(m_miv) + ")";
    }

    GroupedImage::GroupedImage(QImage& image, quint8 k, QObject* parent) : QObject(parent)
    {
    	setObjectName("GroupedImage");
        m_logger = new Logger(this);
        
        m_initialWidth = image.width();
        m_initialHeight = image.height();
        m_initialFormat = image.format();
        
        if (m_initialWidth % PixelGroup::width() != 0)
            m_width = (int) floor((double)m_initialWidth / PixelGroup::width()) + 1;
        else
            m_width = m_initialWidth / PixelGroup::width();

        if (m_initialHeight % PixelGroup::height() != 0)
            m_height = (int) floor((double)m_initialHeight / PixelGroup::height()) + 1;
        else
            m_height = m_initialHeight / PixelGroup::height();

    	
    	m_pg.resize(m_width*m_height);
    	
        compacteAndGroupImage(image, k);
    }
    
    GroupedImage::~GroupedImage()
    {
        /*for (int i=0; i<m_pg.size(); i++)
        {
            if (!m_pg[i].isNull())
            {
                delete m_pg[i];
            }
        }*/
    }

    quint16 GroupedImage::width()
    {
    	return m_width;
    }
    
    quint16 GroupedImage::height()
    {
    	return m_height;
    }
    
    quint16 GroupedImage::initialWidth()
    {
    	return m_initialWidth;
    }
    
    quint16 GroupedImage::initialHeight()
    {
    	return m_initialHeight;
    }

    QPointer<PixelGroup> GroupedImage::pixelGroup(quint16 x, quint16 y)
    {
    	return m_pg[x+(y*m_width)];
    }

    void GroupedImage::compactImage(QImage& image, quint8 k)
    {
        //QImage* img = new QImage(image.width(), image.height(), image.format());
        for (int y=0; y<image.height(); y++)
        {
            for (int x=0; x<image.width(); x++)
            {
                QPointer<YCbCr> ycbcr = new YCbCr(image.pixel(x, y));
                compactPixel(ycbcr, k);
                image.setPixel(x, y, ycbcr->toRGB());
                delete ycbcr;
            }
        }
    }

    void GroupedImage::compacteAndGroupImage(QImage& image, quint8 k) {

        m_logger->debug("Grouping image...");
        for (int y = 0; y < m_initialHeight; y += PixelGroup::height())
        {
            for (int x = 0; x < m_initialWidth; x += PixelGroup::width())
            {
                QPointer<PixelGroup> currentPixelGroup = new PixelGroup(this);
                for (int pgY = 0; pgY < PixelGroup::height(); pgY++)
                {
                    for (int pgX = 0; pgX < PixelGroup::width(); pgX++)
                    {
                        QPointer<YCbCr> ycbcr;
                        if (x + pgX >= m_initialWidth || y + pgY >= m_initialHeight)
                        {
                            /*m_logger->debug("Out of bound: "
                                            + QString::number(x + pgX)
                                            + ":"
                                            + QString::number(y + pgY));*/
                            ycbcr = new YCbCr(-1, -1, -1, this);
                        }
                        else
                        {
                            // convert color to YCbCr and compact luminance
                            ycbcr = new YCbCr(image.pixel(x + pgX, y + pgY), this);
                            //compactPixel(ycbcr, k);
                        }
                        currentPixelGroup->setPixel(pgX, pgY, ycbcr);
                    }
                }

                // Compute and set the current pixel group computed to the new image
                int giX = floor(x / (double)PixelGroup::width());
                int giY = floor(y / (double)PixelGroup::height());
                m_pg[giX + (giY * m_width)] = currentPixelGroup;
            }
        }
        m_logger->debug("Grouped image created");
    }

    //! compact luminance (between +K et 255-K)
    void GroupedImage::compactPixel(QPointer<YCbCr> ycbcr, quint8 k)
    {
        ycbcr->setY((double)(k + (ycbcr->y() * (128-k)/128.0) ));
    }

    QImage* GroupedImage::toImage()
    {
        m_logger->debug("Converting grouped image to rgb image...");

        QImage* img = new QImage(m_initialWidth, m_initialHeight, m_initialFormat);
        // iterate on each group
        for (int i=0; i<m_pg.size(); i++)
        {
            int x = (i % m_width) * PixelGroup::width();
            int y = floor(i / (double)m_width) * PixelGroup::height();

            for (int pgY = 0; pgY < m_pg[i]->height(); pgY++)
            {
                for (int pgX = 0; pgX < m_pg[i]->width(); pgX++)
                {
                    QPointer<YCbCr> pixel = m_pg[i]->pixel(pgX, pgY);
                    // if we are in the image space
                    if (!pixel.isNull())
                    {
                        if (pixel->y() != -1
                            && x + pgX < m_initialWidth && y + pgY < m_initialHeight)
                        {
                            // add current pixel to image
                            img->setPixel(x + pgX, y + pgY, pixel->toRGB());
                        }
                    }
                }
            }
        }

        m_logger->debug("Grouped image converted.");
        
        return img;
    }

}
