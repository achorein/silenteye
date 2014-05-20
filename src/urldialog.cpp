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

#include "urldialog.h"

#include "controller.h"
#include <config.h>

namespace SilentEye {

    UrlDialog::UrlDialog(QWidget* parent) : QDialog(parent)
    {
        setupUi(this);
        setObjectName("UrlDialog");
        connect(downloadButton, SIGNAL(clicked()), this, SLOT(download()));
    }

    UrlDialog::~UrlDialog()
    {
        // NOTHING TO DO
    }

    void UrlDialog::showEvent(QShowEvent* event)
    {
        if(!event->spontaneous())
        {
            m_url.clear();
            statusLabel->setText("");
        }
    }

    void UrlDialog::download()
    {
        QUrl url(urlLineEdit->text());
        if (url.isValid()) {
            if (url.scheme().isEmpty()) {
                statusLabel->setText("URL must start with 'http://'");
            } else if (url.scheme() !=  "http") {
                statusLabel->setText("Unsupported scheme: '"+ url.scheme() +"'");
            } else if (url.host().isEmpty()) {
                statusLabel->setText("You must specify hostname");
            }
            else
            {
                m_url = url;
                this->accept();
            }
        } else {
            statusLabel->setText("URL is not valid");
        }
    }

    QUrl UrlDialog::url()
    {
        return m_url;
    }

}
