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
//  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MODULEWIDGETINTERFACE_H_
#define MODULEWIDGETINTERFACE_H_

#include <QWidget>

//! Framework used by SilentEye and these plug-ins
namespace SilentEyeFramework {

    //! Common interface for all SilentEye's plug-in's widget.
    class ModuleWidgetInterface : public QWidget
    {
    public:
        ModuleWidgetInterface(QWidget* parent) : QWidget(parent) {};

        //! Widget is ready for encoding/decoding process
        virtual bool isReady() const = 0;

        //! Export module configuration
        virtual QMap<QString, QString> exportConfiguration() = 0;
        //! Load module configuration
        virtual void loadConfiguration(QMap<QString, QString>) = 0;
    };

}

#endif
