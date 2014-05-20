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

#ifndef IMAGEBMP_H_
#define IMAGEBMP_H_

#define EQUI_NB_STEP_MIN 9

#include <QPointer>
#include <image.h>
#include <logger.h>

using namespace SilentEyeFramework;

namespace SEFormatBMP {

    class ImageBMP : public Image
    {
    public:
        enum HeaderPosition{TOP=1, BOTTOM=2, SIGNATURE=3};
        enum DataDistribution{INLINE=1, EQUI=2};

    private:
        QPointer<Logger> m_logger;

        QImage m_img;
        unsigned short int m_nbBits;
        bool m_useRed;
        bool m_useGreen;
        bool m_useBlue;
        int m_nbColorUsed;
        HeaderPosition m_headerPosition;
        DataDistribution m_distribution;
        unsigned short int m_swap;
        int m_blockWidth;
        int m_blockHeight;
        int m_blockInnerPos;

    public:
        ImageBMP();
        ~ImageBMP();
        ImageBMP(const QString& filePath);
        ImageBMP(const QPixmap& pixmap, QString filePath="Pixmap");
        ImageBMP(const Image& image);
        ImageBMP(Image* image);

        unsigned short int nbBits() const;
        bool setNbBits(const unsigned short int nb);
        void setColorUsed(bool r=true, bool g=true, bool b=true);
        void setDistribution(DataDistribution);
        void setHeaderPosition(HeaderPosition);

        quint32 capacity() const;

        bool loadData();
        bool saveToDir(QString& outputDirPath);

    private:
        bool hideData();
        int changeColor(int color, const int val);
        int computeDistributionStep(quint32 size);
        QPoint* computeHeaderPosition();
        bool isBetweenPoint(const QPoint&, const QPoint&, const QPoint&);
        QPoint computeNewPosition(const QPoint&, int, bool=false);

    };
}

#endif

