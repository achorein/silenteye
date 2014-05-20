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

#include <QNetworkProxy>
#include <QNetworkAccessManager>
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

        QPointer<Logger> m_logger;

        Controller(QObject* parent=0);

    public:
        ~Controller();

        Config config;
        QString appPath;
        QNetworkAccessManager networkManager;

        static QPointer<Controller> instance();

    public slots:
        void updateProxySettings();

    private slots:
        void proxyAuthentication(const QNetworkProxy&, QAuthenticator*);
    };
}

#endif
