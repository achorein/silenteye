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

#ifndef OPTIONWIDGET_H_
#define OPTIONWIDGET_H_

#include <QtGui>
#include "ui_optionwidget.h"
#include <modulewidgetinterface.h>

#include "imagejpeg.h"

#include <logger.h>
#include <QPointer>

//! SilentEye official module for support of JPEG image
namespace SEFormatJPEG {

    //! Widget given to SilentEye main application in order to configure plugin
    class OptionWidget : public ModuleWidgetInterface, public Ui::OptionWidgetSilentEyeJpeg
    {
        Q_OBJECT;

    private:
        QPointer<Logger> m_logger;

    public:
        OptionWidget(QWidget* parent=0);
        ~OptionWidget();

        int k() const;
        QString passphrase() const;
        int quality() const;
        ImageJPEG::HeaderPosition headerPosition() const;

        void hideQuality(bool=true);

        //! Widget is ready for encoding/decoding process
        bool isReady() const;
        //! Export module configuration
        QMap<QString, QString> exportConfiguration();
        //! Load module configuration
        void loadConfiguration(QMap<QString, QString> map);


    protected slots:
        void pwdStateChange(int);

    signals:
        void optionHasChanged();
    };

}

#endif
