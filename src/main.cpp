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

#include "logger.h"
#include "mainwindow.h"
#include "modulemanager.h"
#include "modulecommander.h"
#include "controller.h"
#include <audio.h>

#include "conf.h"

using namespace SilentEyeFramework;
using namespace SilentEye;

//! Get help message for command line
// \return the help message
QString help() {
    return QString("Usage: [--batch] [action] [file_path] [--message] [message_text] [--message_file] [message_file_path] [--password] [password] [--logpath] [log_path] [--confpath] [conf_path]\n")
            + "\taction\tencode/decode in order to open specific dialog\n"
            + "\tfile_path\tabsolute path to media\n"
            + "\t--batch\tactive batch mode\n"
            + "\t--message\tmessage to hide (encode action)\n"
            + "\t--message_file\tfile to hide (encode action)\n"
            + "\t--password\tpassword uses for encryption\n"
            + "\t--logpath\tabsolute path where log file will be saved\n"
            + "\t--confpath\tabsolute path from where silenteye.conf will be loaded\n"
            + "\t--help\tshow help message";
}

//! Display message box when wrong parameter are given
/*! \param msg Text message to display
  \param logger Logger used for log writing
*/
void showMessage(QString msg, Logger* logger = NULL, bool gui=true)
{
    QString usage = help();
    if (logger != NULL) {
        //logger->info(usage);
        logger->warning(msg);
    } else {
        qWarning() << msg;
    }
    if (gui) {
        QMessageBox mb("SilentEye Warning",
                   ">Error: " + msg + "\n\n" + usage, QMessageBox::Warning, 0, 0, 0);
        mb.exec();
    }
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

    QString conf_path = "";
    QString log_path = "";

    bool batch_mode = false;
    QString message_text = "";
    QString message_file_path = "";
    QString password = "";

    QStringList arguments = app.arguments();
    // Proceed optional options
    for(int i=1; i<arguments.size(); i++) {
        if (arguments.at(i) == "--help") {
            qDebug() << help();
            return 0;
        } else if (arguments.at(i) == "--logpath") {
            arguments.removeAt(i);
            if (i >= arguments.size() || !QFile::exists(arguments.at(i))) {
                showMessage("You must specify a valid path after --logpath option", NULL, !batch_mode);
                return 4;
            }
            log_path = arguments.at(i);
            if (!log_path.endsWith("/")) {
                log_path += "/";
            }
            arguments.removeAt(i);
            i = i-2;
        } else if (arguments.at(i) == "--confpath") {
            arguments.removeAt(i);
            if (i >= arguments.size() || !QFile::exists(arguments.at(i))) {
                showMessage("You must specify a valid path after --confpath option", NULL, !batch_mode);
                return 5;
            }
            conf_path = arguments.at(i);
            if (!conf_path.endsWith("/")) {
                conf_path += "/";
            }
            arguments.removeAt(i);
            i = i-2;
        } else if (arguments.at(i) == "--message") {
            arguments.removeAt(i);
            if (i >= arguments.size()) {
                showMessage("You must specify a text after --message option", NULL, !batch_mode);
                return 6;
            }
            message_text = arguments.at(i);
            arguments.removeAt(i);
            i = i-2;
        } else if (arguments.at(i) == "--message_file") {
            arguments.removeAt(i);
            if (i >= arguments.size() || !QFile::exists(arguments.at(i))) {
                showMessage("You must specify a valid path after --message_file option", NULL, !batch_mode);
                return 7;
            }
            message_file_path = arguments.at(i);
            arguments.removeAt(i);
            i = i-2;
        } else if (arguments.at(i) == "--password") {
            arguments.removeAt(i);
            if (i >= arguments.size() || !QFile::exists(arguments.at(i))) {
                showMessage("You must specify a text after --password option", NULL, !batch_mode);
                return 7;
            }
            password = arguments.at(i);
            arguments.removeAt(i);
            i = i-2;
        } else if (arguments.at(i) == "--batch") {
            arguments.removeAt(i);
            batch_mode = true;
            i = i-1;
        }
    }

    Controller::instance()->appPath = qApp->applicationDirPath()+"/";

    if (log_path == "") {
        #ifdef LOGPATH_USERHOME
            log_path = QDir::homePath()+"/.silenteye/";
        #elif defined(LOGPATH_VALUE)
            log_path = LOGPATH_VALUE + QString("/");
        #else
            log_path = Controller::instance()->appPath;
        #endif
        if (!QFile::exists(log_path)) {
            QDir dir;
            dir.mkdir(log_path);
        }
    }
    
    qDebug() << "Log file path is " + log_path;
    Logger::setFileName(log_path + "silenteye.log");

    if (conf_path == "") {
        #ifdef CONFPATH_USERHOME
            conf_path = QDir::homePath()+"/.silenteye/";
        #elif defined(CONFPATH_VALUE)
            conf_path = CONFPATH_VALUE + QString("/");
        #else
            conf_path = Controller::instance()->appPath;
        #endif
        if (!QFile::exists(conf_path)) {
            QDir dir;
            dir.mkdir(conf_path);
        }
    }
    
    qDebug() << "Configuration path is " + conf_path;
    Controller::instance()->config = Config(conf_path, "silenteye");
    
    Logger::setLevel(Controller::instance()->config.get("loglevel"));

    ModuleManager::load();

    if (arguments.size() <= 1)
    {
        /* Normal mode */
        if( Controller::instance()->config.isEmpty("output") )
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
        Logger logger("main");
        for (int i = 0; i < arguments.size(); i++)
            logger.debug("Parameter " + QString::number(i) + ": " + arguments.at(i));

        QString action;
        QString filePath;

        if(arguments.size() == 2) {
            action = "open";
            filePath = arguments.at(1);
        } else if(arguments.size() == 3) {
            action = arguments.at(1);
            filePath = arguments.at(2);
        } else {
            showMessage("Invalid number of arguments", &logger, !batch_mode);
            return 1;
        }

        if( Controller::instance()->config.isEmpty("output"))
            // default output is the current directory
            Controller::instance()->config.set("output", QDir::currentPath());

        if (!QFile::exists(filePath)) {
            showMessage("Cannot load '" + filePath
                        + "' file (file not found).", &logger, !batch_mode);
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
                            + e.details() + ")", &logger, !batch_mode);
                return 2;
            }
        } else {
            md = new Image(filePath);
            if(((Image*)md.data())->isNull())
            {
                showMessage("Cannot load '" + filePath
                            + "'\n file: unsupported media type.", &logger, !batch_mode);
                return 2;
            }
        }

        int return_code = 0;
        if (batch_mode) {
            logger.info("Running in batch mode");
            if (action == "encode") {
                if (message_text != "")
                    return ModuleCommander::encode(md, message_text, password, Controller::instance()->config.getBool("compressdata"));
                else
                    return ModuleCommander::encode(md, new QFile(message_file_path), password, Controller::instance()->config.getBool("compressdata"));
            } else if(action == "decode") {
                return ModuleCommander::decode(md, password, Controller::instance()->config.getBool("compressdata"));
            } else {
                showMessage("Specified action not recognised: " + action, &logger, !batch_mode);
                return 3;
            }
        } else {
            // create the specified dialog
            OptionDialog* dialog = NULL;
            if (action == "encode") {
                logger.debug("Open encode dialog");
                dialog = new EncodeDialog();
                if (message_text != "")
                    dialog->setMessage(message_text);
                if (message_file_path != "")
                    dialog->setMessageFile(message_file_path);
            } else if(action == "decode") {
                logger.debug("Open decode dialog");
                dialog = new DecodeDialog();
            } else {
                showMessage("Specified action not recognised: " + action, &logger, !batch_mode);
                return 3;
            }

            // setup dialog
            dialog->setMedia(md);

            return_code = dialog->exec();
            delete dialog;
        }
        return return_code;
    }
}

