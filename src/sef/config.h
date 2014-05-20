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
    class Config : public QObject
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
        Config(QString filePath, QString filename, bool=false, QObject* parent=0);
        Config(const QString& content, QObject* parent=0);
        Config(const Config& config);
        ~Config();

        QString filename() const;
        QString filepath() const;
        QString fileAbsoluteName() const;
        QMap<QString, QString> values() const;

        void set(const QString name, const QString value);

        QString get(const QString name) const;
        bool getBool(const QString name) const;
        int getInt(const QString name) const;

        bool contains(const QString name) const;
        bool isEmpty(const QString name) const;

        void save();
        bool isLoaded() const;

        Config& operator=(const Config& config);

    private:
        bool load();

        //public:
        //   static QString path(){ return "config/"; };
    };

}

#endif
