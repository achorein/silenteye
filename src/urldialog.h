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
//  along with SilentEye.  If not, see <http://www.gnu.org/licenses/>.

#ifndef URLDIALOG_H_
#define URLDIALOG_H_

#include <QDialog>
#include <QPointer>
#include <QShowEvent>
#include <QNetworkReply>

#include "ui_urldialog.h"
#include <logger.h>

using namespace SilentEyeFramework;

namespace SilentEye {

    //! Open URL (HTTP)
    class UrlDialog : public QDialog, public Ui::UrlDialog
    {
        Q_OBJECT;

        QUrl m_url;

    public:
        UrlDialog(QWidget* parent=0);
        ~UrlDialog();

        QUrl url();

    private slots:
        void download();

        void showEvent(QShowEvent*);
    };

}

#endif
