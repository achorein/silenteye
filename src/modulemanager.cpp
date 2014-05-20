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

#include "modulemanager.h"
#include <imagemoduleinterface.h>
#include <audiomoduleinterface.h>
#include <videomoduleinterface.h>

#include "conf.h"

namespace SilentEye {

    Logger ModuleManager::m_logger("ModuleManager");
    QList<ModuleInterface*> ModuleManager::m_modulesList;
    QMap< ModuleManager::Category, QMap<QString, ModuleInterface*> > 
            ModuleManager::m_modulesMap;
    QMap<QString, ModuleInterface*> ModuleManager::m_emptyMap;

    //! Get full list 
    /*!
        \return  module's list
    */
    QList<ModuleInterface*>& ModuleManager::getList()
    {
        return m_modulesList;
    }

    //! Retreive module for specific category 
    /*!
        \param cat Category 
        \return Map<name, module>
    */
    QMap<QString, ModuleInterface*>& 
            ModuleManager::get(Category cat)
    {
        if(m_modulesMap.contains(cat))
            return m_modulesMap[cat];

        return m_emptyMap;
    }

    //! Get specific module
    /*!
        \param cat Category
        \param type module type (ex: BMP)
        \return 
    */
    ModuleInterface* 
            ModuleManager::get(Category cat, QString type)
    {
        if(m_modulesMap.contains(cat) && m_modulesMap[cat].contains(type))
            return m_modulesMap[cat][type];

        return NULL;
    }

    //! Is specific module available ?
    /*!
        \param cat Category
        \param type module type (ex: BMP)
        \return
    */
    bool ModuleManager::contains(Category cat, QString type)
    {
        return m_modulesMap.contains(cat) && m_modulesMap[cat].contains(type);
    }

    //! 
    /*!
        \param module Module
        \return Category of the module
    */
    ModuleManager::Category ModuleManager::getCategory(ModuleInterface* module)
    {
        QMap<Category, QMap<QString, ModuleInterface*> >::const_iterator moduleMap = m_modulesMap.constBegin();
        while( moduleMap != m_modulesMap.constEnd() )
        {
            QMap<QString,ModuleInterface*>::const_iterator i = moduleMap.value().constBegin();
            while( i != moduleMap.value().constEnd() )
            {
                if( i.value() == module )
                    return moduleMap.key();
                ++i;
            }
            ++moduleMap;
        }

        return C_UNDEF;
    }

    //! 
    /*!
        \return number of module
    */
    int
            ModuleManager::count()
    {
        return m_modulesList.count();
    }

    //! 
    /*!
        \return number of module of a category
    */
    int 
            ModuleManager::count(Category cat)
    {
        if(m_modulesMap.contains(cat))
            return m_modulesMap[cat].count();
        return 0;
    }

    //! Load SilentEye's plug-ins (modules)
    void ModuleManager::load()
    {
        /* Load static module */
        foreach (QObject *plugin, QPluginLoader::staticInstances())
        {
             loadPlugin(plugin);
        }

        /* Load dynamical module */
        QDir pluginsDir;
	#ifdef LIBPATH_VALUE
	  pluginsDir = QDir(LIBPATH_VALUE + QString("/"));
	#else
	  pluginsDir = QDir(qApp->applicationDirPath());
	#endif
        pluginsDir.cd("modules");
	m_logger.info("Plug-ins dir: " + pluginsDir.absolutePath());

        /* Load all modules in modules directory */
        foreach (QString fileName, pluginsDir.entryList(QDir::Files))
        {
            if(!QLibrary::isLibrary(fileName))
            {
	        m_logger.warning(fileName + " is not a loadable library!");
                continue;
            }
            QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
            m_logger.debug("Loading module " + fileName + "...");
            QObject *plugin = loader.instance();
            loadPlugin(plugin, "[" + fileName + "]" + loader.errorString());
        }
    }

    /*!
        \param plugin Module
        \param error Error message 
    */
    void ModuleManager::loadPlugin(QObject* plugin, QString error)
    {
        if( plugin )
        {
            ImageModuleInterface* moduleI = qobject_cast<ImageModuleInterface*>(plugin);
            if( moduleI )
            {
                if (m_modulesMap[IMAGEFORMAT].contains(moduleI->typeSupported()))
                {
                    m_logger.debug(moduleI->typeSupported() + " already supported, '" + moduleI->name() + "' ignored.");
                    return;
                }
                m_modulesMap[IMAGEFORMAT][moduleI->typeSupported()] = moduleI;
                m_modulesList << moduleI;
                m_logger.debug("Image format module '" + moduleI->name() + "' loaded.");
                return;
            }

            //plugin = loader.instance();
            AudioModuleInterface* moduleA = qobject_cast<AudioModuleInterface*>(plugin);
            if( moduleA )
            {
                if (m_modulesMap[AUDIOFORMAT].contains(moduleA->typeSupported()))
                {
                    m_logger.debug(moduleA->typeSupported() + " already supported, '" + moduleA->name() + "' ignored.");
                    return;
                }
                m_modulesMap[AUDIOFORMAT][moduleA->typeSupported()] = moduleA;
                m_modulesList << moduleA;
                m_logger.debug("Audio format module '" + moduleA->name() + "' loaded.");
                return;
            }

            //plugin = loader.instance();
            VideoModuleInterface* moduleV = qobject_cast<VideoModuleInterface*>(plugin);
            if( moduleV )
            {
                if (m_modulesMap[VIDEOFORMAT].contains(moduleV->typeSupported()))
                {
                    m_logger.debug(moduleV->typeSupported() + " already supported, '" + moduleV->name() + "' ignored.");
                    return;
                }
                m_modulesMap[VIDEOFORMAT][moduleV->typeSupported()] = moduleV;
                m_modulesList << moduleV;
                m_logger.debug("Video format module '" + moduleV->name() + "' loaded.");
                return;
            }

            //plugin = loader.instance();
            CryptoModuleInterface* moduleC = qobject_cast<CryptoModuleInterface*>(plugin);
            if( moduleC )
            {
                if (m_modulesMap[CRYPTO].contains(moduleC->typeSupported()))
                {
                    m_logger.debug(moduleC->typeSupported() + " already supported, " + moduleC->name() + " ignored.");
                    return;
                }
                m_modulesMap[CRYPTO][moduleC->typeSupported()] = moduleC;
                m_modulesList << moduleC;
                m_logger.debug("Cryptography module '" + moduleC->name() + "' loaded.");
                return;
            }
            m_logger.info("Unreconisgned SilentEye's module type: " + QString::fromLatin1(plugin->metaObject()->className()));
        } else {
            m_logger.warning("No plug-in instance returned, detail: " + error);
        }
    }

}
