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

#ifndef DATA_H_
#define DATA_H_

#include <QObject>
#include <QPointer>
#include <QString>
#include <QByteArray>
#include "logger.h"

namespace SilentEyeFramework {

    //! Data encapsulation    
    class Data : public QObject
    {
    public:
        //! Data format
        enum DataFormat{
            BYTES=0, /*!< raw data */
            UINT32=1, /*!< unsigned int 32bit */
            UTF8=2, /*!< String using UF8. */
            LATIN1=3, /*!< String using latin */
            ASCII=4, /*!< String using ascii table */
            FILE=5, /*!< File */
            F_UNDEF=7 }; /*!< undefined format (default) */

    private:
        //! Logger
        QPointer<Logger> m_logger;

        //! Current data format
        DataFormat m_format;
        //! Specific name (ex: filename)
        QString m_name;
        //! Information data
        QByteArray m_data;

    public:
        Data(const DataFormat);
        Data(const QByteArray&, const DataFormat=F_UNDEF);
        Data(const DataFormat, const QByteArray&, const QString="");
        ~Data();
        
        DataFormat format() const;
        QString name() const;
        QByteArray data() const;
        
        void setFormat(const DataFormat);
        void setName(const QString);
        void setData(const QByteArray&);
        
        QByteArray toByteArray() const;
    };
    
}

#endif
