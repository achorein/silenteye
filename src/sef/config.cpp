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

#include "config.h"

namespace SilentEyeFramework {

    Config::Config(QObject* parent)
        : QObject(parent)
    {
        setObjectName("Config");
        m_logger = new Logger(this);
        m_isLoaded = false;
        m_filename = "se-noname.conf";
        m_filepath = "/tmp/";
    }


    Config::Config(QFileInfo file, QObject* parent)
        : QObject(parent)
    {
        setObjectName("Config");
        m_logger = new Logger(this);

        m_filename = file.fileName();
        m_filepath = file.path() + "/";

        m_isLoaded = load();
    }

    Config::Config(QString filePath, QString filename,
                   bool hasExt, QObject* parent)
                       : QObject(parent)
    {
        setObjectName("Config");
        m_logger = new Logger(this);
        if (hasExt) {
            m_filename = filename;
        } else {
            m_filename = filename+".conf";
        }

        m_filepath = filePath;
        if (m_filepath.at(m_filepath.length()-1) != '/') {
            m_filepath += "/";
        }

        m_isLoaded = load();
    }

    Config::Config(const QString& content, QObject* parent)
        : QObject(parent)
    {
        setObjectName("Config");
        m_logger = new Logger(this);
        m_filename = "se-noname.conf";
        m_filepath = "/tmp/";

        m_content = content;
        m_isLoaded = load();
    }

    Config::Config(const Config& config)
    {
        setObjectName("Config");
        m_logger = new Logger(this);
        m_filename = config.filename();
        m_valueMap = config.values();
    }

    Config::~Config(){
        if (!m_logger.isNull())
        {
            delete m_logger;
        }
    }

    QString Config::filename() const
    {
        return m_filename;
    }

    QString Config::filepath() const
    {
        return m_filepath;
    }

    QString Config::absoluteFilePath() const
    {
        return m_filepath + m_filename;
    }

    QMap<QString, QString> Config::values() const
    {
        return m_valueMap;
    }

    bool Config::isLoaded() const
    {
        return m_isLoaded;
    }

    bool Config::contains(QString name) const
    {
        return m_valueMap.contains(name);
    }

    void Config::set(const QString name, const bool value, bool encrypt)
    {
        set(name, QString(value?"1":"0"), encrypt);
    }

    void Config::set(const QString name, const int value, bool encrypt)
    {
        set(name, QString::number(value), encrypt);
    }

    void Config::set(QString name, QString value, bool encrypt)
    {
        if (encrypt)
            m_valueMap[name] = QString::fromUtf8(value.toUtf8().toBase64());
        else
            m_valueMap[name] = value;
    }

    QString Config::get(QString name, bool encrypted) const
    {
        if (m_valueMap.contains(name))
        {
            QString value = m_valueMap.value(name);
            if (encrypted) {
                value = QString::fromUtf8(QByteArray::fromBase64(value.toUtf8()));
            }
            return value;
        }
        else {
            return QString();
        }
    }

    bool Config::save(QString docName)
    {
        QDomDocument doc(docName);
        QDomElement root = doc.createElement("configuration");
        doc.appendChild(root);

        QMap<QString, QString>::const_iterator i = m_valueMap.constBegin();
        while (i != m_valueMap.constEnd()) {
            QDomElement conf = doc.createElement(i.key());
            conf.appendChild(doc.createTextNode(i.value()));
            root.appendChild(conf);
            ++i;
        }

        /* open the output file */
        QFile file(m_filepath+m_filename);
        if( !file.open( QIODevice::WriteOnly ) )
        {
            m_logger->warning("Can't save configuration file to "
                              + m_filepath + m_filename);
            return false;
        }

        /* write data to the output file */
        QTextStream ts( &file );
        /* Profile serialize datas */
        ts << doc.toString();
        file.close();
        return true;
    }

    bool Config::load()
    {
        QDomDocument doc( "SilentEye" );

        if (!m_content.isEmpty()) {
            if(!doc.setContent( m_content ) )
            {
                m_logger->warning("Can't load buffer content to xml document: "
                                  + m_content);
                return false;
            }
        } else {
            QFile file(m_filepath+m_filename);
            if(file.open( QIODevice::ReadOnly ) )
            {
                /* load file content to xml document */
                if(!doc.setContent( &file ) )
                {
                    m_logger->warning("Can't load file content to xml document: "
                                      + m_filepath + m_filename);
                    file.close();
                    return false;
                }
                file.close();
            }
            else
            {
                m_logger->info("Can't open configuration file: "
                                  + m_filepath + m_filename);
                return false;
            }
        }

        QDomElement root = doc.documentElement();

        QDomNode n = root.firstChild();

        while(!n.isNull())
        {
            QDomElement e = n.toElement();
            if( !e.isNull() )
            {
                m_valueMap[e.tagName()] = e.text();
            }
            n = n.nextSibling();
        }
        return true;
    }

    Config& Config::operator=(const Config& config)
                             {
        m_filename = config.filename();
        m_filepath = config.filepath();
        m_valueMap = config.values();
        return *this;
    }

}
