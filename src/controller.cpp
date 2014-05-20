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
        QDir tmpDir(tmpPath);
        if (tmpDir.exists()) {
            QStringList list = tmpDir.entryList(QDir::Files);
            for (int i = 0; i < list.size(); ++i) {
                QString filePath = tmpPath + list.at(i);

                if (QFile::remove(filePath))
                    m_logger->debug("deleted file " + filePath);
                else
                    m_logger->warning("cannot delete file " + filePath);
            }
        }

        if (!m_logger.isNull())
            delete m_logger;
    }

    QPointer<Controller> Controller::instance()
    {
        if (m_pointer.isNull()) {
            m_pointer = new Controller();
        }
        return m_pointer;
    }

    void Controller::httpDownload(QUrl url)
    {
        m_httpUrl = url;
        m_httpFilename = tmpPath
                + QString::number(QDateTime::currentMSecsSinceEpoch())
                + "." + url.path().section('.', -1, -1);

        m_logger->info("Download " + m_httpUrl.toString() + " to " + m_httpFilename);

        m_httpReply = networkManager.get(QNetworkRequest(m_httpUrl));
        connect(m_httpReply, SIGNAL(finished()),
                this, SLOT(httpFinished()));
        connect(m_httpReply, SIGNAL(readyRead()),
                this, SLOT(httpReadyRead()));
    }

    void Controller::httpReadyRead()
    {
        //m_logger->debug("ReadyRead");
        QFile file(m_httpFilename);
        file.open(QIODevice::Append);
        file.write(m_httpReply->readAll());
        file.close();
    }

    void Controller::httpFinished()
    {
        m_logger->debug("Finished");

        QVariant redirectionTarget = m_httpReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        if (m_httpReply->error()) {
            m_logger->error("Download failed: " + m_httpReply->errorString());
            emit httpError(m_httpUrl.toString(), m_httpReply->errorString());
        } else if (!redirectionTarget.isNull()) {
            QUrl newUrl = m_httpUrl.resolved(redirectionTarget.toUrl());
            m_logger->debug("redirect: " + newUrl.toString());
            m_httpReply->deleteLater();
            httpDownload(newUrl);
            return;
        } else {
            emit httpFile(m_httpFilename);
        }

        m_httpReply->deleteLater();
    }

    void Controller::updateProxySettings()
    {
        if (config.get("proxyenable").toInt() == 1) {
            QNetworkProxyFactory::setUseSystemConfiguration(true);
        } else if (config.get("proxyenable").toInt() == 2) {
            QNetworkProxyFactory::setUseSystemConfiguration(false);
            QNetworkProxy proxy;
            proxy.setType(QNetworkProxy::Socks5Proxy);
            proxy.setHostName(config.get("proxyhost"));
            proxy.setPort(config.get("proxyport").toInt());
            if (config.get("proxyloginenable").toInt() == 1)
            {
                proxy.setUser(config.get("proxyuser"));
                proxy.setPassword(config.get("proxypassword", true));
            }
            networkManager.setProxy(proxy);
        } else {
            QNetworkProxyFactory::setUseSystemConfiguration(false);
            QNetworkProxy proxy;
            networkManager.setProxy(proxy);
        }

    }

    void Controller::proxyAuthentication(const QNetworkProxy & proxy, QAuthenticator * authenticator)
    {
        if (config.get("proxyenable").toInt() == 2 && config.get("proxyloginenable").toInt() == 1)
        {
            // authenticator->setOption(???);
            authenticator->setUser(config.get("proxyuser"));
            authenticator->setPassword(config.get("proxypassword", true));
        }
        m_logger->warning("http proxy authentication required ! ("
                          + QString::number((int)proxy.capabilities()) + ")");
    }
}
