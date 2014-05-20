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

#ifndef FORMATMODULEINTERFACE_H_
#define FORMATMODULEINTERFACE_H_

#include "modulewidgetinterface.h"
#include "moduleinterface.h"
#include "media.h"

namespace SilentEyeFramework {

    //! Common interface for media format plug-ins
    class FormatModuleInterface : public ModuleInterface
    {
    public:
        virtual ModuleWidgetInterface* encodeWidget() = 0;
        virtual ModuleWidgetInterface* decodeWidget() = 0;

    signals:
        void optionChanged();
    };

}

#endif
