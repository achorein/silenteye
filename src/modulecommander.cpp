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

#include "controller.h"
#include "modulemanager.h"
#include <imagemoduleinterface.h>
#include <audiomoduleinterface.h>
#include <videomoduleinterface.h>

#include "modulecommander.h"

namespace SilentEye {

    Logger ModuleCommander::m_logger("ModuleCommander");

    //! Encode message into a media
    /*!
        \param md media
        \param message message to hide
        \param password crypt password
        \param password have to compress
    */
    int ModuleCommander::encode(QPointer<Media> md, QString message, QString password, bool compress){
        m_logger.debug("Message to hide: " + message);
        QPointer<EncodedData> data = new EncodedData(message, Data::UTF8, compress);
        return encode(md, data, password, compress);
    }

    //! Encode file into a media
    /*!
        \param md media
        \param file file to hide
        \param password crypt password
        \param password have to compress
    */
    int ModuleCommander::encode(QPointer<Media> md, QFile* file, QString password, bool compress){
        m_logger.debug("File to hide: " + file->fileName());
        QPointer<EncodedData> data = new EncodedData(*file, compress);
        return encode(md, data, password, compress);
    }

    //! Encode message into a media
    /*!
        \param md media
        \param data data to hide
        \param password crypt password
        \param password have to compress
    */
    int ModuleCommander::encode(QPointer<Media> md, QPointer<EncodedData> data, QString password, bool compress){
        int retval = 0;
        CryptoModuleInterface* module;
        try
        {
            if (!password.isEmpty()) {
                m_logger.debug("Crypto required");
                module = (CryptoModuleInterface*) ModuleManager::get(ModuleManager::CRYPTO, Controller::instance()->config.get("encryption"));
                if(module!=NULL)
                {
                    data = module->encode(password, data);
                    data->setCompressed(compress, true);
                }
                else
                    m_logger.warning("Invalid encryption type : ignored");
            }

            if (md->type() == Media::IMAGE) {
                if  (!ModuleManager::contains(ModuleManager::IMAGEFORMAT, Controller::instance()->config.get("imageformat"))) {
                    m_logger.error("Module " + Controller::instance()->config.get("imageformat") + " is not available");
                    return 1;
                }
                Image* img = ((ImageModuleInterface*) ModuleManager::get(ModuleManager::IMAGEFORMAT, Controller::instance()->config.get("imageformat")))->encodeImage(QPointer<Image>((Image*)md.data()), false);
                md = QPointer<Media>(img);
            } else if (md->type() == Media::AUDIO) {
                if  (!ModuleManager::contains(ModuleManager::AUDIOFORMAT, Controller::instance()->config.get("audioformat"))) {
                    m_logger.error("Module " + Controller::instance()->config.get("audioformat") + " is not available");
                    return 1;
                }
                md = ((AudioModuleInterface*) ModuleManager::get(ModuleManager::AUDIOFORMAT, Controller::instance()->config.get("audioformat")))->encodeAudio(QPointer<Audio>((Audio*)md.data()), false);
            } else if (md->type() == Media::VIDEO) {
                if  (!ModuleManager::contains(ModuleManager::VIDEOFORMAT, Controller::instance()->config.get("videoformat"))) {
                    m_logger.error("Module " + Controller::instance()->config.get("videoformat") + " is not available");
                    return 1;
                }
                md = ((VideoModuleInterface*) ModuleManager::get(ModuleManager::VIDEOFORMAT, Controller::instance()->config.get("videoformat")))->encodeVideo(QPointer<Video>((Video*)md.data()), false);
            }

            QString output = Controller::instance()->config.get("output");
            m_logger.info("output directory: " + output);
            m_logger.debug(output+md->shortName() + " == " + md->filePath());

            if (md->filePath().compare(output+md->shortName(), Qt::CaseInsensitive) == 0)
            {
                m_logger.error("Cannot save new file into source directory, please select an other destination!");
                return 11;
            }
            else if (QFile::exists(output+md->shortName()))
            {
                m_logger.error("Cannot save new file : destination file already exists !");
                return 12;
            }

            md->setEncodedData(data);

            QString errorMsg;
            if( md->saveToDir(output) )
            {
                //emit encodedImage(md->filePath());
            }
            else {
                errorMsg += "- An error occured during the encoding process.\n";
                errorMsg += "- Cannot save the output file : "+md->filePath()+".\n";
            }

            if(errorMsg.length() > 0) {
                m_logger.error(errorMsg);
                retval = 13;
            }

        }
        catch(ModuleException e)
        {
            if (module != NULL)
                m_logger.error(module->name() + ": " + e.message() + " -> " + e.details());
            else
                m_logger.error("Module unknown: " + e.message() + " -> " + e.details());
            retval = 14;

        }
        catch(SilentEyeException e)
        {
            m_logger.error(module->name() + ": " + e.message() + " -> " + e.details());
            retval = 15;
        }
        return retval;
    }

    //! Decode message from a media
    /*!
        \param md media
        \param message message to hide
        \param password crypt password
    */
    int ModuleCommander::decode(QPointer<Media> md, QString password, bool compress) {
        int retval = 0;
        CryptoModuleInterface* module = NULL;
        try
        {
            // downcast to daughter class
            if (md->type() == Media::IMAGE) {
                if  (!ModuleManager::contains(ModuleManager::IMAGEFORMAT, Controller::instance()->config.get("imageformat"))) {
                    m_logger.error("Module " + Controller::instance()->config.get("imageformat") + " is not available");
                    return 1;
                }
                Image* img = ((ImageModuleInterface*) ModuleManager::get(ModuleManager::IMAGEFORMAT, Controller::instance()->config.get("imageformat")))->decodeImage(QPointer<Image>((Image*)md.data()));
                md = QPointer<Media>(img);
            } else if (md->type() == Media::AUDIO) {
                if  (!ModuleManager::contains(ModuleManager::AUDIOFORMAT, Controller::instance()->config.get("audioformat"))) {
                    m_logger.error("Module " + Controller::instance()->config.get("audioformat") + " is not available");
                    return 1;
                }
                md = ((AudioModuleInterface*) ModuleManager::get(ModuleManager::AUDIOFORMAT, Controller::instance()->config.get("audioformat")))->decodeAudio(QPointer<Audio>((Audio*)md.data()));
            } else if (md->type() == Media::VIDEO) {
                if  (!ModuleManager::contains(ModuleManager::VIDEOFORMAT, Controller::instance()->config.get("videoformat"))) {
                    m_logger.error("Module " + Controller::instance()->config.get("videoformat") + " is not available");
                    return 1;
                }
                md = ((VideoModuleInterface*) ModuleManager::get(ModuleManager::VIDEOFORMAT, Controller::instance()->config.get("videoformat")))->decodeVideo(QPointer<Video>((Video*)md.data()));
            }

            if(md->loadData())
            {
                QPointer<EncodedData> data = md->encodedData();
                data->setCompressed(compress);

                if(!password.isEmpty())
                {
                    module = (CryptoModuleInterface*) ModuleManager::get(ModuleManager::CRYPTO, Controller::instance()->config.get("encryption"));
                    if(module!=NULL)
                    {
                        QPointer<EncodedData> dataCrypto = module->decode(password, data);
                        delete data;
                        data = dataCrypto;
                        md->setEncodedData(data);
                    }
                    else
                        m_logger.warning("Invalid encryption type : ignored");
                }

                switch(data->format())
                {
                case Data::FILE: {
                    QString outputfilepath = Controller::instance()->config.get("output") + "/" + data->toData()->name();
                    m_logger.info("Decoded file: " + outputfilepath);
                    QFile file(outputfilepath);
                    file.open(QFile::WriteOnly);
                    file.write(data->toData()->data());
                    file.close(); }
                    break;
                case Data::ASCII:
                case Data::UTF8:
                case Data::LATIN1:
                    m_logger.info("Decoded message: " + data->toString());
                    break;
                default:
                    m_logger.error("This media don't seem to have an hidden message.");
                    retval = 16;
                }

            } else {
                m_logger.error("This media don't seem to have an hidden message.");
                retval = 17;
            }

        }
        catch(ModuleException e)
        {
            if (module != NULL)
                m_logger.error(module->name() + ": " + e.message() + " -> " + e.details());
            else
                m_logger.error("Module unknown: " + e.message() + " -> " + e.details());
            retval = 18;
        }
        catch(SilentEyeException e)
        {
            m_logger.error(module->name() + ": " + e.message() + " -> " + e.details());
            retval = 19;
        }
        return retval;
    }
}
