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

#include "media.h"

namespace SilentEyeFramework {

    Media::Media()
    {
        setObjectName("Media");
        m_type = UNKNOW;
        m_shortName = "unamed";
        m_filePath = "/home/";
        m_isLoaded = false;
    }

    Media::Media(const QString& filePath)
    {
        setObjectName("Media");
        m_type = UNKNOW;
        m_filePath = filePath;
        m_shortName = fileName(filePath);
        m_isLoaded = false;
    }

    Media::Media(const Media& md)
    {
        setObjectName("Media");
        m_type = UNKNOW;
        m_data = md.encodedData();
        m_shortName = md.shortName();
        m_filePath = md.filePath();
        m_isLoaded = false;
    }

    Media::Media(Media* md)
    {
        setObjectName("Media");
        m_type = UNKNOW;
        m_shortName = md->shortName();
        m_filePath = md->filePath();
        m_isLoaded = false;
    }

    Media::~Media()
    {
        // NOTHING TO DO
    }

    Media::Type Media::type() const {
        return m_type;
    }

    QPointer<EncodedData> Media::encodedData() const
    {
        return m_data;
    }

    void Media::setEncodedData(QPointer<EncodedData> data)
    {
        if(!m_data.isNull())
            delete m_data;
        m_data = data;
    }

    QString Media::shortName() const
    {
        return m_shortName;
    }

    void Media::setShortName(const QString shortName)
    {
        m_shortName = shortName;
    }

    QString Media::filePath() const
    {
        return m_filePath;
    }

    bool Media::isDataLoaded() const
    {
        return m_isLoaded;
    }

    quint32 Media::capacity() const
    {
        return 0;
    }

    bool Media::loadData()
    {
        return false;
    }

    bool Media::saveToDir(QString& outputDirPath)
    {
        Q_UNUSED(outputDirPath);
        return false;
    }

    QString Media::baseName() const
    {
        return QDir::fromNativeSeparators(m_filePath).section("/", 0, -2);
    }

    QString Media::fileName(QString filePath)
    {
        return QDir::fromNativeSeparators(filePath).section("/", -1, -1);
    }

    void Media::computeNewFileName(QString extension)
    {
        m_shortName = m_shortName.section('.', 0, -2)+"."+extension;
        m_filePath = baseName()+"/"+m_shortName;
    }

}
