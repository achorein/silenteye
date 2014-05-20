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

#ifndef IMAGEJPEG_H_
#define IMAGEJPEG_H_

#include <image.h>
#include <encodeddata.h>
#include <QDebug>

#define EQUI_NB_STEP_MIN 9

using namespace SilentEyeFramework;

namespace SEFormatJPEG {

    //! JPEG image representation
    class ImageJPEG : public Image
    {
    public:
        enum HeaderPosition{TOP=1 /*!< hide header at begenning of the image*/, 
            BOTTOM=2 /*!< hide header at the end of the image*/, 
            SIGNATURE=3 /*!< hide header at 95% of height and width*/};

    private:
        //! logger
        QPointer<Logger> m_logger;

        //! number of pixel to hide into one block
        quint16 m_nbBits;
        //! luminance interval (used for stegotables)
        int m_k;
        //! passphrase for random stegotable
        QString m_passphrase;
        //! JPEG compression quality
        int m_quality;
        //! Header position (for data size)
        HeaderPosition m_headerPosition;
        //! Block width used for equi distribution
        int m_blockWidth;
        //! Block height used for equi distribution
        int m_blockHeight;
        //! Information's position into a block (equi distribution)
        int m_blockInnerPos;

    public:
        //! Create empty instance
        ImageJPEG();
        ~ImageJPEG();
        //! Create instance from file
        ImageJPEG(const QString& filePath);
        //! Create instance from pixmap
        ImageJPEG(const QPixmap& pixmap, QString filePath="Pixmap");
        //! Create instance by copy
        ImageJPEG(const Image& image);
        //! Create instance by reference
        ImageJPEG(Image* image);

        void setK(int);
        void setPassphrase(QString);
        void setQuality(int);
        void setHeaderPosition(HeaderPosition);

        //! JPEG image capacity (octets)
        quint32 capacity() const;

        //! load hidden information from current image
        bool loadData();
        //! Hide information into image and save it to output dir
        /*!
          \param outputDirPath Output directory path for encoded image
        */
        bool saveToDir(QString&);

    private:
        void init();
        //! Compute distribution step according to data size and image dimension
        /*!
          \param size Data size
          \param width Number of block in a row
          \param height Number of block in a column
        */
        int computeDistributionStep(quint32, int, int);
        
        //! Compute header position (bottom/stop/signature)
        /*!
          \param width Number of block in a row
          \param height Number of block in a column
        */
        QPoint* computeHeaderPosition(int, int);
        
        //! Check point collision (used when header is signature)
        /*!
          \param ref Reference point
          \param start First point of segment
          \param end End point of segment
        */
        bool isBetweenPoint(const QPoint&, const QPoint&, const QPoint&);
        
        //! Compute next position according to distribution step and block size
        /*!
          \param oldPos Old position
          \param step Distribution step value
          \param width Number of block in a row
          \param height Number of block in a column
          \param first Compute first position
        */
        QPoint computeNewPosition(const QPoint&, int, int width, int height, bool=false);
    };
}
#endif
