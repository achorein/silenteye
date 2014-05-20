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

#ifndef PROPERTYDIALOG_H_
#define PROPERTYDIALOG_H_

#include <QtGui>
#include <image.h>
#include <logger.h>
#include "ui_propertydialog.h"

using namespace SilentEyeFramework;

namespace SilentEye {

    //! Show current media properties
    class PropertyDialog : public QDialog, public Ui::PropertyDialog
    {
        Q_OBJECT;

    private:
        QPointer<Logger> m_logger;
	QMap<QString, long> m_capacityMap;

        Media* m_md;

    public:
        PropertyDialog(QWidget* parent=0);
        ~PropertyDialog();

        void setMedia(Media*);

    private slots:
	void formatChanged(const QString&);
    };
}

#endif
