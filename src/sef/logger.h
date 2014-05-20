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

#ifndef LOGGER_H
#define LOGGER_H

#include <QPointer>
#include <QObject>
#include <QFile>
#include <QString>

namespace SilentEyeFramework {

    //! Provides methods for multi-level logging into logfile and console
    class Logger : public QObject
    {
    public:
        enum LogLevel{ DEBUG_LEVEL=1, INFO_LEVEL=5, WARNING_LEVEL=10, ERROR_LEVEL=15 };

    private:
        static QPointer<QFile> m_file;
        static int m_nbInstance;
        static QString m_logFileName;
        static LogLevel m_level;
        QString m_className;

    public:
        Logger(QObject* parent = 0);
        Logger(QString className);
        ~Logger();

        QString className() const;
        void setClassName(const QString&);

        void debug(QString);
        void info(QString);
        void warning(QString);
        void error(QString);

        static void setFileName(const QString&);
        static void setLevel(Logger::LogLevel=DEBUG_LEVEL);
        static void setLevel(const QString&);

    private:
        void init();
        void write(QString priority, QString text);
    };

}

#endif // LOGGER_H
