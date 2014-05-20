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

#ifndef MODULEMANAGER_H_
#define MODULEMANAGER_H_

#include <QtCore>
#include <moduleinterface.h>
#include <formatmoduleinterface.h>
#include <cryptomoduleinterface.h>
#include <logger.h>

using namespace SilentEyeFramework;

#ifdef QT_STATICPLUGIN
    Q_IMPORT_PLUGIN(secryptoaes)
    Q_IMPORT_PLUGIN(secryptoaes256)
    Q_IMPORT_PLUGIN(seformatbmp)
    Q_IMPORT_PLUGIN(seformatjpeg)
    Q_IMPORT_PLUGIN(seformatwav)
#endif

namespace SilentEye {

    //! Load and manage SilentEye's Plug-ins
    class ModuleManager
    {
    public:
        //! Plug-in category
        enum Category{ 
	    IMAGEFORMAT, /*!< Module for image */
	    AUDIOFORMAT, /*!< Module for audio */
	    VIDEOFORMAT, /*!< Module for video */
	    CRYPTO, /*!< Cryptography (AES, etc)*/
	    C_UNDEF }; /*!< undefined category */

    private:
	//! Logger
        static Logger m_logger;
	//! Loaded module map (cached) by Category
        static QMap<Category, QMap<QString, ModuleInterface*> > m_modulesMap;
	//! Empty Map for unknowned Category
        static QMap<QString, ModuleInterface*> m_emptyMap;
	//! Global module list (all categories)
        static QList<ModuleInterface*> m_modulesList;

    public:
        static QList<ModuleInterface*>& getList();
        static QMap<QString, ModuleInterface*>& get(Category cat);
        static ModuleInterface* get(Category cat, QString type);
        static bool contains(Category cat, QString type);

        static Category getCategory(ModuleInterface*);

        static int count();
        static int count(Category cat);

        static void load();

    private:
        static void loadPlugin(QObject*, QString="Unknown");

    };
}

#endif
