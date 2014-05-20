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

#ifndef _SILENTEYE_EXCEPTION_H_
#define _SILENTEYE_EXCEPTION_H_

#include <QtCore>

namespace SilentEyeFramework {

    //! Internal exception only used by SilentEyeFramework
    class SilentEyeException : public QtConcurrent::Exception
    {
    protected:
        QString m_message;
        QString m_details;

    public:
        SilentEyeException(const QString& message) throw();
        SilentEyeException(const QString& message, const QString& details) throw();

        SilentEyeException(const SilentEyeException& exception) throw();
        ~SilentEyeException() throw();

        QString message() const;
        QString details() const;

        void raise() const{ throw *this; }
        Exception *clone() const{ return new SilentEyeException(*this); }
    };

}

#endif
