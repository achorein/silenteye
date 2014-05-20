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

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <QMutex>

#include <QNetworkProxy>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QAuthenticator>

#include <config.h>
#include <logger.h>

using namespace SilentEyeFramework;

namespace SilentEye {

    //! Manage HTTP proxy and application path (singleton)
    class Controller : public QObject
    {
        Q_OBJECT;

    private:
        static QPointer<Controller> m_pointer;

        QUrl m_httpUrl;
        QString m_httpFilename;
        QNetworkReply* m_httpReply;
        QPointer<Logger> m_logger;

        Controller(QObject* parent=0);

    public:
        ~Controller();

        Config config;
        QString appPath;
        QString tmpPath;
        QString translationsPath;
        QNetworkAccessManager networkManager;

        static QPointer<Controller> instance();

    public slots:
        void updateProxySettings();
        //! Download file from http source
        /*!
            \param url file URL (http://)
        */
        void httpDownload(QUrl);

    private slots:
        void proxyAuthentication(const QNetworkProxy&, QAuthenticator*);

        void httpReadyRead();
        void httpFinished();

    signals:
        void httpFile(QString);
        void httpError(QString, QString);
    };
}

#endif
