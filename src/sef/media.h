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

#ifndef MEDIA_H_
#define MEDIA_H_

#include <QPointer>
#include <QString>
#include <QDir>
#include "encodeddata.h"

namespace SilentEyeFramework {

    //! Generic representation of Media which can load and hide specific data according to it's format
    class Media : public QObject
    {
    public:
        enum Type{ IMAGE, AUDIO, VIDEO, UNKNOW };

    protected:
        //! Media type (Image/Audio/Video)
        Type m_type;
        //! Embedded data
        QPointer<EncodedData> m_data;
        //! Media file path
        QString m_filePath;
        //! Media name
        QString m_shortName;
        //! Indicate when Media contains some data
        bool m_isLoaded;

    public:
        Media();
        ~Media();
        Media(const QString& filepath);
        Media(const Media& img);
        Media(Media* img);

        Type type() const;

        QPointer<EncodedData> encodedData() const;
        void setEncodedData(QPointer<EncodedData> data);

        QString filePath() const;
        QString baseName() const;
        QString shortName() const;
        void setShortName(const QString shortName);

        bool isDataLoaded() const;

        virtual quint32 capacity() const;

        virtual bool loadData();
        virtual bool saveToDir(QString& outputDirPath);

    protected:
        QString fileName(QString filePath);
        void computeNewFileName(QString extension);
    };

}

#endif
