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

#ifndef CONFIG_H_
#define CONFIG_H_

#include <QtCore>
#include <QDomDocument>
#include <QDomElement>

#include "logger.h"

namespace SilentEyeFramework {

    //! Manage configuration for application (read/write in xml file)
    class Config : QObject
    {
    private:
        bool m_isLoaded;
        QString m_filename;
        QString m_filepath;
        QString m_content;
        QMap<QString, QString> m_valueMap;
        QPointer<Logger> m_logger;

    public:
        Config(QObject* parent=0);

        //! Create a configuration from existing file
        /*!
            \param absolutefilePath absolute path including the file name
            \param parent Object's parent
        */
        Config(QFileInfo file, QObject* parent=0);
        //! Create a configuration from path and name
        /*!
            \param filePath file's path. This doesn't include the file name
            \param name configuration name or filename (by default .conf is append to the name in order to generate filename)
            \param hasExt true when given name already have an extension (ex: .conf)
            \param parent Object's parent
        */
        Config(QString filePath, QString name, bool hasExt=false, QObject* parent=0);
        //! Create a configuration from XML raw
        /*!
            \param content String representing XML (ex: "<root></root>")
            \param parent Object's parent
        */
        Config(const QString& content, QObject* parent=0);

        Config(const Config& config);
        ~Config();

        //! Returns the name of the file, excluding the path.
        QString filename() const;
        //! Returns the file's path. This doesn't include the file name.
        QString filepath() const;
        //! Returns an absolute path including the file name. The absolute path name consists of the full path and the file name
        QString absoluteFilePath() const;
        //! Returns raw map containing values
        QMap<QString, QString> values() const;

        //! Add or override a string value
        /*!
            \param name associating key
            \param value value associated with the key <i>name</i>
            \param encrypt when true the value is hashed using base64
        */
        void set(const QString name, const bool value, bool encrypt=false);

        //! Add or override a integer value
        /*!
            \param name associating key
            \param value value associated with the key <i>name</i>
            \param encrypt when true the value is hashed using base64
        */
        void set(const QString name, const int value, bool encrypt=false);

        //! Add or override a boulean value
        /*!
            \param name associating key
            \param value value associated with the key <i>name</i>
            \param encrypt when true the value is hashed using base64
        */
        void set(const QString name, const QString value, bool encrypt=false);

        //! Retrieved value according to its associated name
        /*!
            \param name associating key
            \param encrypt when true the value has been hashed using base64
        */
        QString get(const QString name, bool encrypted=false) const;

        //! Returns true if the map contains an item with key <i>name</i>; otherwise returns false.
        bool contains(const QString name) const;

        //! Convert and write change into XML file
        bool save(QString docName = "SilentEye");

        //! Returns true if the file has been successfully load
        bool isLoaded() const;

        Config& operator=(const Config& config);

    private:
        //! Parse XML file and load values
        bool load();

        //public:
        //   static QString path(){ return "config/"; };
    };

}

#endif
