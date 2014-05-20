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
        enum HeaderPosition{TOP=1, BOTTOM=2, SIGNATURE=3, HKEY=4};
        enum DataDistribution{INLINE=1, EQUI=2, DKEY=3};

    private:
	//! Logger
        QPointer<Logger> m_logger;

	//! Representation of the image (loaded BMP file)
        QImage m_img;
	//! Bits per pixel color
        unsigned short int m_nbBits;
	//! indicate if message have to be encoded into the red color
        bool m_useRed;
	//! indicate if message have to be encoded into the green color
        bool m_useGreen;
	//! indicate if message have to be encoded into the blue color
        bool m_useBlue;
	//! number of color used by pixel
        int m_nbColorUsed;
	//! Key used when Data or Header distribution is set to DKEY/HKEY
        QString m_distributionKey;
	//! Indicate header position mode
        HeaderPosition m_headerPosition;
	//! Indicate data distribition mode
        DataDistribution m_distribution;
	//! Swap value for encoding process
        unsigned short int m_swap;
	//! block size for data distribution
        int m_blockWidth;
	//! block size for data distribution
        int m_blockHeight;
	//! hidden information's postion into a block
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
        void setDistributionKey(QString key);
        void setDistribution(DataDistribution);
        void setHeaderPosition(HeaderPosition);

	//! Compute capaity of the image (number of octets which can be hide)
	/*!
	  \return number of octets
	*/
        quint32 capacity() const;

	//! Load hidden data from current image
        bool loadData();

	//! Hide data into image and save file into the output path
	/*!
	  \param outputDirPath Path where encoded image will be saved
	  \return true if image has been saved
	*/
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

