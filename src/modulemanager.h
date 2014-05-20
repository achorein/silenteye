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
        enum Category{ IMAGEFORMAT, AUDIOFORMAT, VIDEOFORMAT, CRYPTO, C_UNDEF };

    private:
        static Logger m_logger;
        static QMap<Category, QMap<QString, ModuleInterface*> > m_modulesMap;
        static QMap<QString, ModuleInterface*> m_emptyMap;
        static QList<ModuleInterface*> m_modulesList;

    public:
        static QList<ModuleInterface*>& getList();
        static QMap<QString, ModuleInterface*>& get(Category cat);
        static ModuleInterface* get(Category cat, QString type);

        static Category getCategory(ModuleInterface*);

        static int count();
        static int count(Category cat);

        static void load();

    private:
        static void loadPlugin(QObject*, QString="Unknown");

    };
}

#endif
