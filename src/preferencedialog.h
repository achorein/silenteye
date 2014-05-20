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

#ifndef PREFERENCEDIALOG_H_
#define PREFERENCEDIALOG_H_

#include <QtGui>
#include <QPointer>
#include <logger.h>
#include "ui_preferencedialog.h"

using namespace SilentEyeFramework;

namespace SilentEye {

    //! Preference/configuration dialog
    class PreferenceDialog : public QDialog, public Ui::PreferenceDialog
    {
        Q_OBJECT;

    private:
        QPointer<Logger> m_logger;

        QMap<QString, int> m_imageFormatMap;
        QMap<QString, int> m_audioFormatMap;
        QMap<QString, int> m_cryptoMap;

    public:
        PreferenceDialog(QWidget* parent=0);
        ~PreferenceDialog();

    private:
        void setConfig();

    private slots:
        void accepted();
        void rejected();
        void selectFile();
        void useProxy();
        void useLoginProxy(int);
    };

}

#endif

