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

#include "imagejpeg.h"
#include "moduleexception.h"
#include <math.h>

#include "stegotable.h"
#include "groupedimage.h"

namespace SEFormatJPEG {

    ImageJPEG::ImageJPEG()
        : Image()
    {
        init();
    }

    ImageJPEG::ImageJPEG(const QString& filePath)
        : Image(filePath)
    {
        init();
    }

    ImageJPEG::ImageJPEG(const QPixmap& pixmap, QString filePath)
        : Image(pixmap, filePath)
    {
        init();
    }

    ImageJPEG::ImageJPEG(const Image& img)
        : Image(img)
    {
        init();
    }

    ImageJPEG::ImageJPEG(Image* img)
        : Image(img)
    {
        init();
    }

    ImageJPEG::~ImageJPEG()
    {
        delete m_logger;
    }

    void ImageJPEG::init()
    {
        computeNewFileName("jpg");
        setObjectName("ImageJPEG");
        m_logger = new Logger(this);
        m_nbBits = 1;
        m_k = 20;
        m_passphrase = "SilentEye";
        m_quality = 50;
        m_headerPosition = BOTTOM;
    }

    void ImageJPEG::setK(int k)
    {
        m_k = k;
    }

    void ImageJPEG::setPassphrase(QString pass)
    {
        m_passphrase = pass;
    }

    void ImageJPEG::setQuality(int quality)
    {
        m_quality = quality;
    }

    void ImageJPEG::setHeaderPosition(HeaderPosition pos)
    {
        m_headerPosition = pos;
    }

    quint32 ImageJPEG::capacity() const
    {
        int nbPixelAvailable = (imgWidth() * imgHeight());
	int capacity = floor((floor(nbPixelAvailable / 64.0) - 32) / 8.0);
        return (capacity<0) ? 0 : capacity;
    }

    bool ImageJPEG::loadData()
    {
        m_logger->debug("loading");
        m_nbBits = 1;
        m_isLoaded = false;
        EncodedData dataSize(Data::UINT32);
        dataSize.initialize(m_nbBits);

        if (m_data.isNull())
            m_data = new EncodedData();
        else
            m_data->clear();

        m_data->initialize(m_nbBits);
        int x=0, y=0;

        StegoTable steganoTable(m_passphrase, m_k, this);

        QImage image = this->toImage();
        GroupedImage* gimg = new GroupedImage(image, steganoTable.k(), this);

        QPoint* headerPos = computeHeaderPosition(gimg->width(), gimg->height());
        x = headerPos[0].x();
        y = headerPos[0].y();

        int nbBitsRead = 0;
        while(nbBitsRead<32 && x<gimg->width() && y<gimg->height())
        {
            QPointer<PixelGroup> pg = gimg->pixelGroup(x, y);
            float miv = pg->miv();
            if (miv >= 0 && miv < 256)
            {
                m_logger->debug("pos: " + QString::number(x) + "," + QString::number(y)/* + ". loaded:" + pg->toString()*/ + " => " + QString::number(miv));
                int val = steganoTable.computeValue(miv) ? 1 : 0;
                dataSize.append(val);
                nbBitsRead += m_nbBits;
            } else {
                m_logger->warning("miv is invalid, pos: " + QString::number(x) + "," + QString::number(y));
            }

            x += 1;
            if(x >= gimg->width())
            {
                y++;
                x=0;
            }
        }

        quint32 nbOctets = dataSize.toUInt32();
        m_logger->debug("loaded size: " + QString::number(nbOctets));

        if(m_headerPosition != TOP)
        {
            x = 0;
            y = 0;
        }

        if (nbOctets > capacity()) {
            m_logger->warning("data size > capacity ("
                              + QString::number(nbOctets)
                              + " > " + QString::number(capacity()) + ")");
            delete gimg;
            return false;
        }

        int step = computeDistributionStep(nbOctets, gimg->width(), gimg->height());
        QPoint newPos = computeNewPosition(QPoint(x, y), step, gimg->width(), gimg->height(), true);
        x = newPos.x();
        y = newPos.y();

        m_logger->debug("first loaded pixel(step: " + QString::number(step) + "): "
                        + QString::number(x) + ":" + QString::number(y));

        int nbBits = nbOctets*8;
        nbBitsRead = 0;
        while(nbBitsRead < nbBits && x<gimg->width() && y<gimg->height())
        {
            if(isBetweenPoint(QPoint(x, y), headerPos[0], headerPos[1]))
            {
                // skip header block
                m_logger->debug("skip: " + QString::number(x) + "," + QString::number(y));
                QPoint pos = computeNewPosition(headerPos[1], 1, gimg->width(), gimg->height());
                x = pos.x();
                y = pos.y();
                continue;
            }
            else
            {
                QPointer<PixelGroup> pg = gimg->pixelGroup(x, y);
                float miv = pg->miv();
                if (miv >= 0 && miv < 256)
                {
                    int val = steganoTable.computeValue(miv) ? 1 : 0;
                    m_data->append(val);
                    nbBitsRead += m_nbBits;
                } else {
                    m_logger->warning("miv is invalid, pos: " + QString::number(x) + "," + QString::number(y));
                }
            }

            // change cursor to the next pixel
            if(nbBitsRead < nbBits)
            {
                QPoint newPos = computeNewPosition(QPoint(x, y), step, gimg->width(), gimg->height());
                x = newPos.x();
                y = newPos.y();
            }
        }

        m_logger->debug("last loaded pixel: "
                        + QString::number(x) + ":" + QString::number(y));
        delete(headerPos);

        m_isLoaded = m_data->size()>0;
        delete gimg;
        return m_isLoaded;
    }

    bool ImageJPEG::saveToDir(QString& outputDirPath)
    {
        int x=0, y=0;

        if (m_data.isNull())
            throw ModuleException("Technical error during encoding process",
                                  "Cannot insert null data into image");

        if (m_data->size() > capacity()) {
            m_logger->warning("data size > capacity ("
                              + QString::number(m_data->size())
                              + " > " + QString::number(capacity()) + ")");
            return false;
        }

    	m_filePath = outputDirPath+"/"+m_shortName;
        StegoTable steganoTable(m_passphrase, m_k, this);

        QImage image = this->toImage();
        GroupedImage::compactImage(image, steganoTable.k());

        if(!image.save(m_filePath, "JPEG", m_quality))
        {
            m_logger->warning("Cannot save image to " + m_filePath);
            return false;
        }
        load(m_filePath);

        image = this->toImage();
        GroupedImage* gimg = new GroupedImage(image, steganoTable.k(), this);

        EncodedData sizeData(m_data->size());
        m_logger->debug("Setted size: " + QString::number(sizeData.toUInt32()) 
                        + "/" + QString::number(capacity()));

        QPoint* headerPos = computeHeaderPosition(gimg->width(), gimg->height());
        x = headerPos[0].x();
        y = headerPos[0].y();

        sizeData.initialize(m_nbBits);
        while(sizeData.hasNext() && x<gimg->width() && y<gimg->height())
        {
            QPointer<PixelGroup> pg = gimg->pixelGroup(x, y);
            float miv = pg->miv();
            if (miv >= 0 && miv < 256)
            {
                int val = sizeData.read();
                float miv2 = steganoTable.computeNewMiv(miv, val!=0);
                pg->updateMivTo(miv2);
                m_logger->debug("pos: " + QString::number(x) + "," + QString::number(y)
                                + " => " + QString::number(miv) + " to " + QString::number(miv2) + ", "
                                + "Result:" + QString::number(pg->miv()) );
            } else {
                m_logger->warning("miv is invalid, pos: " + QString::number(x) + "," + QString::number(y));
            }
            
            x += 1;
            if(x >= gimg->width())
            {
                y++;
                x=0;
            }
        }

        if(m_headerPosition != TOP)
        {
            x = 0;
            y = 0;
        }

        int step = computeDistributionStep(m_data->size(), gimg->width(), gimg->height());
        QPoint newPos = computeNewPosition(QPoint(x, y), step, gimg->width(), gimg->height(), true);
        x = newPos.x();
        y = newPos.y();

        m_logger->debug("first setted pixel(step: " + QString::number(step) + "): "
                        + QString::number(x) + ":" + QString::number(y));

        m_data->initialize(m_nbBits);
        while(m_data->hasNext() && x<gimg->width() && y<gimg->height())
        {
            if(isBetweenPoint(QPoint(x, y), headerPos[0], headerPos[1]))
            {
                // skip header block
                m_logger->debug("skip: " + QString::number(x) + "," + QString::number(y));
                QPoint pos = computeNewPosition(headerPos[1], 1, gimg->width(), gimg->height());
                x = pos.x();
                y = pos.y();
                continue;
            }
            else
            {
                QPointer<PixelGroup> pg = gimg->pixelGroup(x, y);
                float miv = pg->miv();
                if (miv >= 0 && miv < 256)
                {
                    int val = m_data->read();
                    float miv2 = steganoTable.computeNewMiv(miv, val!=0);
                    pg->updateMivTo(miv2);
                    m_logger->debug("pos: " + QString::number(x) + "," + QString::number(y)
                                    + " => " + QString::number(miv) + " to " + QString::number(miv2) + ", "
                                    + "Result:" + QString::number(pg->miv()) );
                } else {
                    m_logger->warning("miv is invalid, pos: " + QString::number(x) + "," + QString::number(y));
                }
            }

            if(m_data->hasNext())
            {
                QPoint newPos = computeNewPosition(QPoint(x, y), step, gimg->width(), gimg->height());
                x = newPos.x();
                y = newPos.y();
            }
        }
        delete(headerPos);

        m_logger->debug("last setted pixel: "
                        + QString::number(x) + ":" + QString::number(y));

        QImage* img = gimg->toImage();
        m_logger->debug("saving image...");
        bool ok = img->save(m_filePath, "JPEG", m_quality);
        m_logger->debug("image saved");
        delete img;
        m_logger->debug("image deleted");
        delete gimg;
        m_logger->debug("gimg deleted");

        if (x>=gimg->width() || y>=gimg->height())
        {
            m_logger->warning("Data too large (" + QString::number(x) + ":" + QString::number(y) + ")");
            return false;
        }

        if (ok) {
            load(m_filePath);
            m_logger->debug("image loaded");
        }
        return ok;

    }

    int ImageJPEG::computeDistributionStep(quint32 size, int width, int height)
    {
        int step = 1;
        int sizeNbPixel = 32;
        int nbPixelAvailable = (width * height) - sizeNbPixel;
        int nbPixelData = size * 8;

        step = floor(((double)nbPixelAvailable) / nbPixelData);

        if(step <= 0)
        {
            m_logger->debug("computed step was 0 => set 1");
            step = 1;
        }
        else if(step >= EQUI_NB_STEP_MIN)
        {
            int squareLength = floor(sqrt(step));
            //double q = (height / ceil((sizeNbPixel*squareLength)/width))*squareLength;

            if (width < height)
            {
                double ratio = ((double)width) / height;
                // ratio *= 1/(ratio*2);
                //double ratio = ((double)width) / (height-q);
                //ratio *= 1/ratio;
                m_blockWidth = ceil(squareLength*ratio);
                m_blockHeight = floor(m_blockWidth/ratio);
            }
            else
            {
                double ratio = ((double)height) / width;
                //double ratio = ((double)(height-q)) / width;
                //ratio *= 1/ratio;
                m_blockHeight = ceil(squareLength*ratio);
                m_blockWidth = floor(m_blockHeight/ratio);
            }

            int nbBlockMaxSquare = floor(floor((double)height/(m_blockHeight)) * floor((double)width/(m_blockWidth)));
            if(nbBlockMaxSquare < nbPixelData)
            {
                m_logger->error("Distribution step failed ! (" + QString::number(nbPixelData) + "/" + QString::number(nbBlockMaxSquare) + ")");
            }

            int posX = floor(m_blockWidth/2.0);
            int posY = floor(m_blockHeight/2.0);
            if (posX <= posY)
            {
                m_blockInnerPos = posX;
            }
            else
            {
                m_blockInnerPos = posY;
            }

            m_logger->debug("BlockWidth: " + QString::number(m_blockWidth) + ", "
                            + "BlockHeight: " + QString::number(m_blockHeight) + ", "
                            + "BlockInnerPos: " + QString::number(m_blockInnerPos) + ", "
                            + "DataNbPixel: " + QString::number(nbPixelData) + ", "
                            + "NbBlockMax: " + QString::number(nbBlockMaxSquare));
        } else {
            m_logger->debug("Keep original step: " + QString::number(step) + "<" + QString::number(EQUI_NB_STEP_MIN));
        }

        return step;
    }

    QPoint* ImageJPEG::computeHeaderPosition(int width, int height)
    {
        int headNbPixel = 32;

        int headStartX = 0;
        int headStartY = 0;

        if(m_headerPosition == BOTTOM)
        {
            headStartX = (abs(headNbPixel - width)) % width;
            headStartY = height - ceil((headStartX + headNbPixel - 1) / (double)width);
        }
        else if(m_headerPosition == SIGNATURE)
        {
            headStartX = floor(width * 0.95);
            headStartY = floor(height * 0.95);
            if (((width - headStartX) + (height - headStartY)*width) < headNbPixel)
                throw ModuleException("Technical error during loading process",
                                      "Image too small for use of signature mode (header position)");
        }

        int headEndX = (headStartX + headNbPixel - 1) % width;
        int headEndY = headStartY + floor((headStartX + headNbPixel - 1) / (double)width);

        m_logger->debug("headNbPixel: " + QString::number(headNbPixel)
                        + ", headStartX: " + QString::number(headStartX)
                        + ", headStartY: " + QString::number(headStartY)
                        + ", headEndX: " + QString::number(headEndX)
                        + ", headEndY: " + QString::number(headEndY));

        QPoint* tab = new QPoint[2];
        tab[0] = QPoint(headStartX, headStartY);
        tab[1] = QPoint(headEndX, headEndY);

        return tab;
    }

    bool ImageJPEG::isBetweenPoint(const QPoint& ref, const QPoint& start, const QPoint& end)
    {
        if(start.y() == end.y()) // singleline
        {
            return ref.y() == start.y() && ref.x() >= start.x() && ref.x() <= end.x();
        }
        else // multiline
        {
            return (ref.y() == start.y() && ref.x() >= start.x())
                    || (ref.y() == end.y() && ref.x() <= end.x())
                    || (ref.y() > start.y() && ref.y() < end.y());
        }
    }

    QPoint ImageJPEG::computeNewPosition(const QPoint& oldPos, int step, int width, int height, bool first)
    {
        QPoint pos(oldPos);
        if(first)
        {
            if (step >= EQUI_NB_STEP_MIN)
            {
                pos.setX(m_blockInnerPos);
                pos.setY(m_blockInnerPos);
            }
            return pos;
        }

        int newX = 0;
        if(step >= EQUI_NB_STEP_MIN)
        {
            newX = oldPos.x() + m_blockWidth;
        }
        else
        {
            newX = oldPos.x() + step;
        }

        if(newX >= width)
        {
            if(step >= EQUI_NB_STEP_MIN)
            {
                pos.ry() += m_blockHeight;
            }
            else
            {
                pos.ry() += floor(newX / (double)width);
            }
        }
        pos.setX(newX % width);

        //m_logger->debug("new position: " + QString::number(pos.x()) + ":" + QString::number(pos.y()));

        return pos;
    }

}
