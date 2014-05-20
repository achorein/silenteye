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

#ifndef CRYPTOMODULEINTERFACE_H_
#define CRYPTOMODULEINTERFACE_H_

#include "moduleinterface.h"
#include "encodeddata.h"

namespace SilentEyeFramework {

    //! Common interface for cryptography plug-ins
    class CryptoModuleInterface : public ModuleInterface
    {
    public:
        //! Encrypte data using a specific key
        virtual QPointer<EncodedData> encode(QString key, QPointer<EncodedData> msg) = 0;
        //! Decrypte data using a specific key
        virtual QPointer<EncodedData> decode(QString key, QPointer<EncodedData> data) = 0;
    };

}

Q_DECLARE_INTERFACE(SilentEyeFramework::CryptoModuleInterface,
                    "org.nikikko.sef.CryptoModuleInterface/1.0");

#endif
