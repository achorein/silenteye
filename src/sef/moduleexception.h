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

#ifndef _MODULE_EXCEPTION_H_
#define _MODULE_EXCEPTION_H_

#include <QtCore>
#include "silenteyeexception.h"

namespace SilentEyeFramework {

    //! Exception used by all SilentEye's plug-in
    class ModuleException : public SilentEyeException
    {
    public:
        ModuleException(const QString& message) throw();
        ModuleException(const QString& message, const QString& details) throw();

        ModuleException(const ModuleException& exception) throw();
        ~ModuleException() throw();
    };

}

#endif
