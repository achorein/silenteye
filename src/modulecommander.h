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

#ifndef MODULECOMMANDER_H
#define MODULECOMMANDER_H

#include <QtCore>
#include <image.h>
#include <moduleinterface.h>
#include <formatmoduleinterface.h>
#include <cryptomoduleinterface.h>
#include <logger.h>

#include "modulemanager.h"

using namespace SilentEyeFramework;

namespace SilentEye {

    class ModuleCommander
    {
    private:
    //! Logger
        static Logger m_logger;

    public:
        static int encode(QPointer<Media>, QString, QString="", bool=false);
        static int encode(QPointer<Media>, QFile*, QString="", bool=false);
        static int decode(QPointer<Media>, QString="", bool=false);

    private:
        static int encode(QPointer<Media>, QPointer<EncodedData>, QString="", bool=false);
    };

}

#endif // MODULECOMMANDER_H
