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

#include "controller.h"

using namespace SilentEyeFramework;

namespace SilentEye {

    QPointer<Controller> Controller::m_pointer = NULL;

    Controller::Controller(QObject *parent) : QObject(parent)
    {
        setObjectName("Controller");
        m_logger = new Logger(this);

        appPath = "/";

        connect(&networkManager, SIGNAL(proxyAuthenticationRequired (const QNetworkProxy&, QAuthenticator*)),
                this, SLOT(proxyAuthentication(const QNetworkProxy&, QAuthenticator*)));
    }

    Controller::~Controller()
    {
        // NOTHING TO DO
    }

    QPointer<Controller> Controller::instance()
    {
        if (m_pointer.isNull()) {
            m_pointer = new Controller();
        }
        return m_pointer;
    }


    void Controller::updateProxySettings()
    {
        QNetworkProxy proxy;
        if (config.get("proxyenable").toInt() == 1)
        {
            proxy.setType(QNetworkProxy::HttpProxy);
            proxy.setHostName(config.get("proxyhost"));
            proxy.setPort(config.get("proxyport").toInt());
        }
        networkManager.setProxy(proxy);
    }

    void Controller::proxyAuthentication(const QNetworkProxy & proxy, QAuthenticator * authenticator)
    {
        m_logger->warning("http proxy authentication required !");
    }
}
