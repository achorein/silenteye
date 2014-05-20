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

#include "imagebmp.h"
#include "moduleexception.h"
#include <math.h>

namespace SEFormatBMP {

    ImageBMP::ImageBMP()
        : Image()
    {
        m_nbBits = 2;
        setColorUsed(true, false, false);
        m_headerPosition = TOP;
        m_distribution = INLINE;
        setObjectName("ImageBMP");
        m_logger = new Logger(this);
    }

    ImageBMP::ImageBMP(const QString& filePath)
        : Image(filePath)
    {
        m_img = this->toImage();
        m_nbBits = 2;
        setColorUsed(true, false, false);
        m_headerPosition = TOP;
        m_distribution = INLINE;
        computeNewFileName("bmp");
        setObjectName("ImageBMP");
        m_logger = new Logger(this);
    }

    ImageBMP::ImageBMP(const QPixmap& pixmap, QString filePath)
        : Image(pixmap, filePath)
    {
        m_img = this->toImage();
        m_nbBits = 2;
        setColorUsed(true, false, false);
        m_headerPosition = TOP;
        m_distribution = INLINE;
        computeNewFileName("bmp");
        setObjectName("ImageBMP");
        m_logger = new Logger(this);
    }

    ImageBMP::ImageBMP(const Image& img)
        : Image(img)
    {
        m_img = this->toImage();
        m_nbBits = 2;
        setColorUsed(true, false, false);
        m_headerPosition = TOP;
        m_distribution = INLINE;
        computeNewFileName("bmp");
        setObjectName("ImageBMP");
        m_logger = new Logger(this);
    }

    ImageBMP::ImageBMP(Image* img)
        : Image(img)
    {
        m_nbBits = 2;
        setColorUsed(true, false, false);
        m_headerPosition = TOP;
        m_distribution = INLINE;
        computeNewFileName("bmp");
        setObjectName("ImageBMP");
        m_logger = new Logger(this);
    }

    ImageBMP::~ImageBMP()
    {
        delete m_logger;
    }

    unsigned short int ImageBMP::nbBits() const{
        return m_nbBits;
    }

    void ImageBMP::setColorUsed(bool r, bool g, bool b)
    {
        m_useRed = r;
        m_useGreen = g;
        m_useBlue = b;
        m_nbColorUsed = 0;
        if (m_useRed)
            m_nbColorUsed++;
        if (m_useGreen)
            m_nbColorUsed++;
        if (m_useBlue)
            m_nbColorUsed++;
    }

    void ImageBMP::setDistribution(DataDistribution value)
    {
        m_distribution = value;
    }

    void ImageBMP::setHeaderPosition(HeaderPosition value)
    {
        m_headerPosition = value;
    }

    bool ImageBMP::setNbBits(const unsigned short int nb)
    {
        if(nb>0 && nb<=6)
        {
            m_nbBits = nb;
            return true;
        }
        else
            m_nbBits = 2;

        m_isLoaded = false;
        return m_isLoaded;
    }

    quint32 ImageBMP::capacity() const
    {
        int sizeNbPixel = ceil(32.0/(m_nbColorUsed*m_nbBits));
        int nbPixelAvailable = (imgWidth() * imgHeight()) - sizeNbPixel;
        return floor((m_nbBits * m_nbColorUsed * nbPixelAvailable) / 8.0);
    }

    bool ImageBMP::loadData()
    {
        m_isLoaded = false;
        EncodedData dataSize(Data::UINT32);
        dataSize.initialize(m_nbBits);

        if (m_data.isNull())
            m_data = new EncodedData();
        else
            m_data->clear();

        m_data->initialize(m_nbBits);
        int x=0, y=0;

        QSize imgSize = m_img.size();
        unsigned short int andOperator = EncodedData::andOperator(m_nbBits);

        QPoint* headerPos = computeHeaderPosition();
        x = headerPos[0].x();
        y = headerPos[0].y();

        int nbBitsRead = 0;
        while(x<imgSize.width() && y<imgSize.height() && nbBitsRead<32)
        {
            QRgb pixel = m_img.pixel(x, y);

            int val;
            if (m_useRed && nbBitsRead<32) {
                val = (qRed(pixel))&andOperator;
                dataSize.append(val);
                nbBitsRead += m_nbBits;
            }
            if (m_useGreen && nbBitsRead<32) {
                val = (qGreen(pixel))&andOperator;
                dataSize.append(val);
                nbBitsRead += m_nbBits;
            }
            if (m_useBlue && nbBitsRead<32) {
                val = (qBlue(pixel))&andOperator;
                dataSize.append(val);
                nbBitsRead += m_nbBits;
            }

            // change cursor to the next pixel
            if(x==imgSize.width()-1)
                y++;
            x=(x+1)%imgSize.width();
        }
        quint32 nbOctets = dataSize.toUInt32();

        m_logger->debug("loaded size: " + QString::number(nbOctets));

        if(m_headerPosition != TOP)
        {
            x = 0;
            y = 0;
        }

        if (nbOctets > capacity())
            return false;

        int step = computeDistributionStep(nbOctets);
        QPoint newPos = computeNewPosition(QPoint(x, y), step, true);
        x = newPos.x();
        y = newPos.y();

        m_logger->debug("first loaded pixel(step: " + QString::number(step) + "): " 
                        + QString::number(x) + ":" + QString::number(y));

        int nbBits = nbOctets*8;
        nbBitsRead = 0;
        while(x<imgSize.width() && y<imgSize.height() &&
              nbBitsRead < nbBits)
        {
            if(isBetweenPoint(QPoint(x, y), headerPos[0], headerPos[1]))
            {
                // skip header block
                m_logger->debug("skip: " + QString::number(x) + "," + QString::number(y));
                QPoint pos = computeNewPosition(headerPos[1], 1);
                x = pos.x();
                y = pos.y();
                continue;
            }
            else
            {
                QRgb pixel = m_img.pixel(x, y);

                int val = 0;
                if (m_useRed && nbBitsRead<nbBits) {
                    val = (qRed(pixel))&andOperator;
                    m_data->append(val);
                    nbBitsRead += m_nbBits;
                }
                if (m_useGreen && nbBitsRead<nbBits) {
                    val = (qGreen(pixel))&andOperator;
                    m_data->append(val);
                    nbBitsRead += m_nbBits;
                }
                if (m_useBlue && nbBitsRead<nbBits) {
                    val = (qBlue(pixel))&andOperator;
                    m_data->append(val);
                    nbBitsRead += m_nbBits;
                }
            }

            // change cursor to the next pixel
            if(nbBitsRead < nbBits)
            {
                QPoint newPos = computeNewPosition(QPoint(x, y), step);
                x = newPos.x();
                y = newPos.y();
            }
        }

        m_logger->debug("last loaded pixel: "
                        + QString::number(x) + ":" + QString::number(y));
        delete(headerPos);

        m_isLoaded = m_data->size()>0;
        return m_isLoaded;
    }

    bool ImageBMP::hideData()
    {
        int x=0, y=0;
        QSize imgSize = m_img.size();
        m_swap = 255 - EncodedData::andOperator(m_nbBits);

        if (m_data.isNull())
            throw ModuleException("Technical error during encoding process",
                                  "Cannot insert null data into image");
        if (m_img.isNull())
            throw ModuleException("Technical error during encoding process",
                                  "Cannot insert data into an empty image");

        EncodedData sizeData(m_data->size());
        m_logger->debug("Setted size: " + QString::number(sizeData.toUInt32()) 
                        + "/" + QString::number(capacity()));

        QPoint* headerPos = computeHeaderPosition();
        x = headerPos[0].x();
        y = headerPos[0].y();

        int nbColorUsed = 0;
        sizeData.initialize(m_nbBits);
        while(sizeData.hasNext() && x<imgSize.width() && y<imgSize.height())
        {
            QRgb pixel = m_img.pixel(x, y);

            int val = sizeData.read();

            if (m_useRed && nbColorUsed == 0) {
                int red = changeColor(qRed(pixel), val);
                m_img.setPixel(x, y, qRgb(red, qGreen(pixel), qBlue(pixel)));
                nbColorUsed++;
            } else if (m_useGreen &&
                       ((nbColorUsed == 0 && !m_useRed)
                        || (m_useRed && nbColorUsed == 1))) {
                int green = changeColor(qGreen(pixel), val);
                m_img.setPixel(x, y, qRgb(qRed(pixel), green, qBlue(pixel)));
                nbColorUsed++;
            } else if (m_useBlue) {
                int blue = changeColor(qBlue(pixel), val);
                m_img.setPixel(x, y, qRgb(qRed(pixel), qGreen(pixel), blue));
                nbColorUsed++;
            }

            // change cursor to the next pixel
            if(nbColorUsed == m_nbColorUsed || !sizeData.hasNext())
            {
                nbColorUsed = 0;
                if(x==imgSize.width()-1)
                    y++;
                x=(x+1)%imgSize.width();
            }
        }

        if(m_headerPosition != TOP)
        {
            x = 0;
            y = 0;
        }

        int step = computeDistributionStep(m_data->size());
        QPoint newPos = computeNewPosition(QPoint(x, y), step, true);
        x = newPos.x();
        y = newPos.y();

        m_logger->debug("first setted pixel(step: " + QString::number(step) + "): " 
                        + QString::number(x) + ":" + QString::number(y));

        m_data->initialize(m_nbBits);
        while(m_data->hasNext() && x<imgSize.width() && y<imgSize.height())
        {

            if(isBetweenPoint(QPoint(x, y), headerPos[0], headerPos[1]))
            {
                // skip header block
                m_logger->debug("skip: " + QString::number(x) + "," + QString::number(y));
                QPoint pos = computeNewPosition(headerPos[1], 1);
                x = pos.x();
                y = pos.y();
                continue;
            }
            else
            {
                QRgb pixel = m_img.pixel(x, y);
                int val = m_data->read();

                /*  nbRead++;
                    QString debug = "compileData[size:" + QString::number(m_data->size())
                    + ",nbBits:" + QString::number(m_nbBits) + "]: ";
                    debug += "nbRead: " + QString::number(nbRead) + ", ";
                    debug += "coord: " + QString::number(x) +":"+ QString::number(y) + ", ";
                    debug += "val: " + QString::number(val) + ", ";
                    debug += "hasNext: " + QString::number(m_data->hasNext()) + ", "; */

                if (m_useRed && nbColorUsed == 0) {
                    //debug += "use of red, ";
                    int red = changeColor(qRed(pixel), val);
                    m_img.setPixel(x, y, qRgb(red, qGreen(pixel), qBlue(pixel)));
                    nbColorUsed++;
                } else if (m_useGreen &&
                           ((nbColorUsed == 0 && !m_useRed)
                            || (m_useRed && nbColorUsed == 1))) {
                    //debug += "use of green, ";
                    int green = changeColor(qGreen(pixel), val);
                    m_img.setPixel(x, y, qRgb(qRed(pixel), green, qBlue(pixel)));
                    nbColorUsed++;
                } else if (m_useBlue) {
                    //debug += "use of blue, ";
                    int blue = changeColor(qBlue(pixel), val);
                    m_img.setPixel(x, y, qRgb(qRed(pixel), qGreen(pixel), blue));
                    nbColorUsed++;
                }
                // m_logger->debug(debug + "nbColor: " + QString::number(nbColorUsed));
            }

            // change cursor to the next pixel
            if(nbColorUsed == m_nbColorUsed && m_data->hasNext())
            {
                nbColorUsed = 0;
                QPoint newPos = computeNewPosition(QPoint(x, y), step);
                x = newPos.x();
                y = newPos.y();
            }
        }

        delete(headerPos);

        if (x>=imgSize.width() || y>=imgSize.height())
        {
            m_logger->warning("Data too large (" + QString::number(x) + ":" + QString::number(y) + ")");
            return false;
        }

        m_logger->debug("last setted pixel: "
                        + QString::number(x) + ":" + QString::number(y));
        return true;
    }

    bool ImageBMP::saveToDir(QString& outputDirPath)
    {
        if(!hideData())
            return false;

        if (!outputDirPath.endsWith('/'))
        {
            outputDirPath += "/";
        }
        m_filePath = outputDirPath+m_shortName;

        if(!m_img.isNull())
        {
            return m_img.save(m_filePath, "BMP", 100);
        }
        else
            m_logger->warning("Cannot save file: image is null.");

        return false;
    }

    int ImageBMP::changeColor(int color, const int val)
    {
        return (color&m_swap)+val;
    }

    int ImageBMP::computeDistributionStep(quint32 size)
    {
        int step = 1;
        if(m_distribution == EQUI)
        {
            int sizeNbPixel = ceil(32.0/(m_nbColorUsed*m_nbBits));
            int nbPixelAvailable = (imgWidth() * imgHeight()) - sizeNbPixel;
            int nbPixelData = ceil((size*8.0)/(m_nbColorUsed*m_nbBits));

            step = floor(((double)nbPixelAvailable) / nbPixelData);

            if(step <= 0)
            {
                m_logger->debug("computed step was 0 => set 1");
                step = 1;
            }
            else if(step >= EQUI_NB_STEP_MIN)
            {

                int squareLength = floor(sqrt(step));
                if (imgWidth() < imgHeight())
                {
                    double ratio = ((double)imgWidth()) / imgHeight();
                    ratio *= 1/(ratio*2);
                    m_blockWidth = ceil(squareLength*ratio);
                    m_blockHeight = ceil(m_blockWidth/ratio);
                }
                else
                {
                    double ratio = ((double)imgHeight()) / imgWidth();
                    ratio *= 1/(ratio*2);
                    m_blockHeight = ceil(squareLength*ratio);
                    m_blockWidth = ceil(m_blockHeight/ratio);
                }

                int nbBlockMaxSquare = floor(floor((double)imgHeight()/(m_blockHeight)) * floor((double)imgWidth()/(m_blockWidth)));
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
            }
        }
        return step;
    }

    QPoint* ImageBMP::computeHeaderPosition()
    {  
        int headNbPixel = ceil(32.0 / (m_nbColorUsed * m_nbBits));

        int headStartX = 0;
        int headStartY = 0;

        if(m_headerPosition == BOTTOM)
        {
            headStartX = (abs(headNbPixel - imgWidth())) % imgWidth();
            headStartY = imgHeight() - ceil((headStartX + headNbPixel - 1) / (double)imgWidth());
        }
        else if(m_headerPosition == SIGNATURE)
        {
            headStartX = floor(imgWidth() * 0.95);
            headStartY = floor(imgHeight() * 0.95);
            if (((imgWidth() - headStartX) + (imgHeight() - headStartY)*imgWidth()) < headNbPixel)
                throw ModuleException("Technical error during loading process",
                                      "Image too small for use of signature mode (header position)");
        }

        int headEndX = (headStartX + headNbPixel - 1) % imgWidth();
        int headEndY = headStartY + floor((headStartX + headNbPixel - 1) / (double)imgWidth());

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

    bool ImageBMP::isBetweenPoint(const QPoint& ref, const QPoint& start, const QPoint& end)
    {
        if(start.y() == end.y()) // singleline
        {
            return ref.y() == start.y() && ref.x() >= start.x() && ref.x() <= end.x();
        }
        else// multiline
        {
            return (ref.y() == start.y() && ref.x() >= start.x())
                    || (ref.y() == end.y() && ref.x() <= end.x())
                    || (ref.y() > start.y() && ref.y() < end.y());
        }
    }

    QPoint ImageBMP::computeNewPosition(const QPoint& oldPos, int step, bool first)
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

        if(newX >= imgWidth())
        {
            if(step >= EQUI_NB_STEP_MIN)
            {
                pos.ry() += m_blockHeight;
            }
            else
            {
                pos.ry() += floor(newX / (double)imgWidth());
            }
        }
        pos.setX(newX % imgWidth());

        //m_logger->debug("new position: " + QString::number(pos.x()) + ":" + QString::number(pos.y()));

        return pos;
    }

}
