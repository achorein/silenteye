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

#include <QApplication>
#include <QTranslator>

#include "logger.h"
#include "mainwindow.h"
#include "modulemanager.h"
#include "controller.h"
#include <audio.h>

using namespace SilentEyeFramework;
using namespace SilentEye;

//! Display message box when wrong parameter are given
/*! \param msg Text message to display
  \param logger Logger used for log writing
*/
void showMessage(QString msg, Logger& logger)
{
    logger.warning(msg);
    QMessageBox mb("SilentEye Warning",
                   msg, QMessageBox::Warning, 0, 0, 0);
    mb.exec();
}

//! Main !
/*!
  \param argc Number of parameter
  \param argv parameter values
*/
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    #ifdef STATICPLUGINS
        Q_INIT_RESOURCE(silenteye);
    #endif

    Controller::instance()->appPath = qApp->applicationDirPath()+"/";
    Controller::instance()->config = Config(Controller::instance()->appPath, "silenteye");
    Controller::instance()->tmpPath = Controller::instance()->appPath + "tmp/";
    Controller::instance()->translationsPath = Controller::instance()->appPath + "translations/";
    
    Logger::setFileName(Controller::instance()->appPath + "silenteye.log");
    Logger::setLevel(Controller::instance()->config.get("loglevel"));

    Logger logger("main");

    if (Controller::instance()->config.get("language") != "" && Controller::instance()->config.get("language") != "en")
    {
        QString languageName = "silenteye_" + Controller::instance()->config.get("language");
        QString languageFile = Controller::instance()->translationsPath + languageName + ".qm" ;

        if (QFile::exists(languageFile))
        {
            QTranslator qtTranslator;
            if(qtTranslator.load(languageName, Controller::instance()->translationsPath))
            {
                app.installTranslator(&qtTranslator);
                logger.info("Language '" + languageName + "' loaded.");
            }
            else {
                logger.info("Cannot load language file '" + languageFile + "'.");
            }
        } else {
            logger.info("Cannot find language file " + languageFile);
        }
    }

    ModuleManager::load();

    if (app.argc() <= 1)
    {
        /* Normal mode */
        if( !Controller::instance()->config.contains("output") || Controller::instance()->config.get("output").trimmed() == "")
            // default output is the user home
            Controller::instance()->config.set("output", QDir::homePath()+"/");

        Controller::instance()->updateProxySettings();

        MainWindow mainWin;
        mainWin.show();
        return app.exec();
    }
    else
    {
        /* Quick mode */
        for (int i = 0; i < app.argc(); i++)
            logger.debug("Parameter " + QString::number(i) + ": " + QString(app.argv()[i]));

        QString action(app.argv()[1]);
        QString filePath(app.argv()[2]);

        if(app.argc() == 2) {
            action = "open";
            filePath = app.argv()[1];
        } else if(app.argc() == 3) {
            action = app.argv()[1];
            filePath = app.argv()[2];
        } else {
            showMessage("Usage: " + QString(app.argv()[0]) 
                        + " <action> [file_path]\n"
                        + "action: encode/decode\n"
                        + "file_path: absolute path to media", logger);
            return 1;
        }

        if( !Controller::instance()->config.contains("output") || Controller::instance()->config.get("output").trimmed() == "")
            // default output is the current directory
            Controller::instance()->config.set("output", QDir::currentPath());

        if (!QFile::exists(filePath)) {
            showMessage("Cannot load '" + filePath
                        + "'\n file: file not found.", logger);
            return 2;
        }
        if(action == "open") {
            Controller::instance()->updateProxySettings();

            MainWindow mainWin(filePath);
            mainWin.show();
            return app.exec();
        }

        // Load Media
        QPointer<Media> md;
        if (filePath.endsWith(".wav", Qt::CaseInsensitive)) {

            try {
                md = new Audio(filePath);
            } catch (SilentEyeException e) {
                showMessage("Cannot load '" + filePath
                            + "'\n file: unsupported media type. ("
                            + e.details() + ")", logger);
                return 2;
            }
        } else {
            md = new Image(filePath);
            if(((Image*)md.data())->isNull())
            {
                showMessage("Cannot load '" + filePath
                            + "'\n file: unsupported media type.", logger);
                return 2;
            }
        }

        // create the specified dialog
        OptionDialog* dialog = NULL;
        if (action == "encode") {
            dialog = new EncodeDialog();
        } else if(action == "decode") {
            dialog = new DecodeDialog();
        } else {
            showMessage("Specified action not recognised: " + action, logger);
            return 3;
        }

        // setup dialog
        dialog->setMedia(md);

        int return_code = dialog->exec();
        delete dialog;
        return return_code;
    }
}

