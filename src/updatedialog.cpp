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

#include "updatedialog.h"

#include "controller.h"
#include <config.h>

#include "conf.h"

namespace SilentEye {

    UpdateDialog::UpdateDialog(QWidget* parent) : QDialog(parent)
    {
        setupUi(this);
        setObjectName("UpdateDialog");
        m_logger = new Logger(this);

	QString cmn_path;
        #if defined(CMNPATH_VALUE)
	   cmn_path = CMNPATH_VALUE + QString("/");
        #else
	   cmn_path = Controller::instance()->appPath;
	#endif
	m_logger->info("Version file path: " + cmn_path);
        Config version(cmn_path, "version.xml", true);
        if (version.isLoaded()) {
            currentVersionLabel->setText(version.get("name"));
            currentDateLabel->setText(version.get("date"));
        } else {
            currentVersionLabel->setText("Unknown");
            currentDateLabel->setText("Unknown");
        }
        linkLabel->setText("<a href='http://www.silenteye.org/download.html?i2&referer=update-default'>http://www.silenteye.org/download.html</a>");
    }

    UpdateDialog::~UpdateDialog()
    {
      //if (!m_logger.isNull()) {
      //   delete m_logger;
      //}
    }

    void UpdateDialog::showEvent(QShowEvent* event)
    {
        if(!event->spontaneous())
        {
            progressBar->setMaximum(0);
            progressBar->setValue(-1);
            download(QUrl(Controller::instance()->config.get("updatelink")));
        }
    }

    void UpdateDialog::download(QUrl url)
    {

        m_url = url;
        m_logger->debug("Download: " + m_url.toString());
        m_buffer = "";
        m_reply = Controller::instance()->networkManager.get(QNetworkRequest(url));
        connect(m_reply, SIGNAL(finished()),
                this, SLOT(httpFinished()));
        connect(m_reply, SIGNAL(readyRead()),
                this, SLOT(httpReadyRead()));
    }

    void UpdateDialog::httpReadyRead()
    {
        m_buffer += QString(m_reply->readAll()).trimmed();
    }

    void UpdateDialog::httpFinished()
    {
        m_logger->debug("Finished");
        progressBar->setMaximum(100);
        QVariant redirectionTarget = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        if (m_reply->error()) {
            m_logger->error("Download failed: " + m_reply->errorString());
            progressBar->setValue(0);

            newVersionLabel->setText("error...");
            dateLabel->setText("error...");
            releaseBrowser->setText("check failed: " + m_reply->errorString());
            linkLabel->setText("<a href='http://www.silenteye.org/download.html?i2&referer=update-error'>http://www.silenteye.org/download.html</a>");
        } else if (!redirectionTarget.isNull()) {
            QUrl newUrl = m_url.resolved(redirectionTarget.toUrl());
            m_logger->debug("redirect: " + newUrl.toString());
            m_reply->deleteLater();
            download(newUrl);
            return;
        } else {
            progressBar->setValue(100);
            Config conf(m_buffer);

            newVersionLabel->setText(conf.get("name"));
            dateLabel->setText(conf.get("date"));
            releaseBrowser->setText(conf.get("releasenote").trimmed());
#if defined(Q_WS_WIN)
            linkLabel->setText("<a href='" + conf.get("link-win") + "'>"
                               + conf.get("link-win") + "</a>");
#elif defined(Q_WS_X11)
            linkLabel->setText("<a href='" + conf.get("link-lin") + "'>"
                               + conf.get("link-lin") + "</a>");
#elif defined(Q_WS_MAC)
            linkLabel->setText("<a href='" + conf.get("link-mac") + "'>"
                               + conf.get("link-mac") + "</a>");
#else
            linkLabel->setText("<a href='http://www.silenteye.org/download.html?i2&referer=update-platform-unknown'>http://www.silenteye.org/download.html</a>");
#endif
        }

        m_reply->deleteLater();
    }


}
