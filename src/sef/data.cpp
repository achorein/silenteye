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

#include "data.h"
#include "silenteyeexception.h"

namespace SilentEyeFramework {
        
        Data::Data(const QByteArray& bytes, const DataFormat format)
        {
            this->setObjectName("Data");
            m_logger = new Logger(this);
            if (format == UINT32)
            {
                m_format = format;
                m_data = bytes;
            }
            else if(bytes.isEmpty())
            {
                m_format = F_UNDEF;
                m_data = bytes;
            }
            else
            {
                QByteArray rawbytes = bytes;
                m_format = (DataFormat) ((int)rawbytes.at(0) - (int)'0');
                rawbytes.remove(0, 1);
                if (m_format < 0 || m_format > 7)
                    throw SilentEyeException("Loaded informations are invalid! Check your options...",
                                             "Data format unknown (" + QString::number(m_format) + ")");
                if (format != F_UNDEF && format != m_format)
                    throw SilentEyeException("Requested format doesn't match loaded informations! Check your options...",
                                             "Data format dismatch (" + QString::number(m_format) + "!=" + QString::number(format) + ")");
                
                if (m_format == FILE)
                {
                    int index = rawbytes.indexOf('<');
                    m_data = rawbytes.right(rawbytes.size() - (index+1));
                    rawbytes.truncate(index);
                    m_name = QString::fromUtf8(rawbytes.data());
                }
                else
                {
                    m_name = "";
                    m_data = rawbytes;
                }
            }
        }

        Data::Data(const DataFormat format)
            : m_format(format)
        {
            this->setObjectName("Data");
            m_logger = new Logger(this);
        }
        
        Data::Data(const DataFormat format, const QByteArray& data, const QString name)
            : m_format(format), m_name(name), m_data(data)
        {
            this->setObjectName("Data");
            m_logger = new Logger(this);
        }

        Data::~Data()
        {
            delete m_logger;
        }
        
        Data::DataFormat Data::format() const
        {
            return m_format;
        }
        
        QString Data::name() const
        {
            return m_name;
        }
        
        QByteArray Data::data() const
        {
            return m_data;
        }
        
        QByteArray Data::toByteArray() const
        {
            QByteArray bytes = m_data;
            switch(m_format)
            {
            case BYTES:
            case UTF8:
            case LATIN1:
            case ASCII:
                bytes.prepend((char)((int)'0' + (int)m_format));
                break;
            case FILE:
                bytes.prepend((m_name+'<').toUtf8());
                bytes.prepend((char)((int)'0' + (int)m_format));
                break;
            case UINT32:
            case F_UNDEF:
                break;
            }
            
            return bytes;
        }
}
