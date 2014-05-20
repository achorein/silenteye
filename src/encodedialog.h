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

#ifndef ENCODEDIALOG_H_
#define ENCODEDIALOG_H_

#include "optiondialog.h"

namespace SilentEye {

    //! Dialog for generic encoding process
    class EncodeDialog : public OptionDialog
    {
        Q_OBJECT;

    public:
        EncodeDialog(QWidget* parent=0);

    protected:
        QWidget* currentFormatWidget(FormatModuleInterface*);

    private slots:
        void ok();
        void showEvent(QShowEvent*);
        void dragEnterEvent(QDragEnterEvent* event);
        void dropEvent(QDropEvent* event);

    signals:
        void encodedImage(QString);
    };
}

#endif
