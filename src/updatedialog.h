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

#ifndef UPDATEDIALOG_H_
#define UPDATEDIALOG_H_

#include <QDialog>
#include <QPointer>
#include <QShowEvent>
#include <QNetworkReply>

#include "ui_updatedialog.h"
#include <logger.h>

using namespace SilentEyeFramework;

namespace SilentEye {

    //! Check for new version of SilentEye (HTTP)
    class UpdateDialog : public QDialog, public Ui::UpdateDialog
    {
        Q_OBJECT;

    private:
        QUrl m_url;
        QNetworkReply* m_reply;
        QString m_buffer;

        QPointer<Logger> m_logger;

    public:
        UpdateDialog(QWidget* parent=0);
        ~UpdateDialog();

    private slots:
        void showEvent(QShowEvent*);

        void download(QUrl);
        void httpReadyRead();
        void httpFinished();
    };

}

#endif
