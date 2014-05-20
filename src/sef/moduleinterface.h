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

#ifndef MODULEINTERFACE_H_
#define MODULEINTERFACE_H_

#include <QObject>
#include "moduleexception.h"

//! Framework used by SilentEye and these plug-ins
namespace SilentEyeFramework {

    //! Common interface for all SilentEye's plug-in.
    class ModuleInterface : public QObject
    {
    public:
        //! Plug-in name (ex: FooBar)
        virtual QString name() const = 0;
        //! Plug-in version (ex: 1.2.0)
        virtual QString version() const = 0;
        //! Type supported (ex: BMP, JPEG, AES128, etc.)
        virtual QString typeSupported() const = 0;
        //! Current status of the module (ex: KO|qca2-plugin-ossl missing)
        virtual QString status() = 0;
    };

}

Q_DECLARE_INTERFACE(SilentEyeFramework::ModuleInterface,
                    "org.silenteye.sef.ModuleInterface/1.0");

#endif
