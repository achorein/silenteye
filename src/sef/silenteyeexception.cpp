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

#include "moduleexception.h"

namespace SilentEyeFramework {

    SilentEyeException::SilentEyeException(const QString& message) throw()
        : Exception() {
        m_message = message;
        m_details = "";
    }

    SilentEyeException::SilentEyeException(const QString& message,
                                           const QString& details) throw()
                                               : Exception(){
        m_message = message;
        m_details = details;
    }

    SilentEyeException::SilentEyeException(const SilentEyeException& exception) throw()
        : Exception(){
        m_message = exception.message();
        m_details = exception.details();
    }

    SilentEyeException::~SilentEyeException() throw(){}

    QString SilentEyeException::message() const{
        return m_message;
    }

    QString SilentEyeException::details() const{
        return m_details;
    }

}
