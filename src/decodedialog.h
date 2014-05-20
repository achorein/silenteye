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

#ifndef DECODEDIALOG_H_
#define DECODEDIALOG_H_

#include <QPointer>

#include <data.h>
#include "optiondialog.h"

namespace SilentEye {

    //! Dialog for generic decoding process
    class DecodeDialog : public OptionDialog
    {
        Q_OBJECT;

    public:
        DecodeDialog(QWidget* parent=0);
        ~DecodeDialog();

    protected:
        QWidget* currentFormatWidget(FormatModuleInterface*);

        private slots:
        void ok();
        void showEvent(QShowEvent*);
        void releaseFileButton();
    };

}

#endif
