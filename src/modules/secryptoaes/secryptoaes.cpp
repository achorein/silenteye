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
//  along with SilentEye.  If not, see <http://www.gnu.org/licenses/>.

#include "secryptoaes.h"
#include "math.h"

namespace SECryptoAES {

    SECryptoAES::SECryptoAES()
    {
        this->setObjectName("SECryptoAES128");
        m_logger = new Logger(this);
        m_init = NULL;
        m_isQcaCompatible = false;
    }

    SECryptoAES::~SECryptoAES()
    {
        if (m_init != NULL)
            delete m_init;
        delete m_logger;
    }

    void SECryptoAES::init()
    {
        if (m_init != NULL)
            return;
        m_init = new QCA::Initializer;
        m_isQcaCompatible = QCA::isSupported("aes128-cbc-pkcs7");

        if( m_isQcaCompatible )
        {
            m_logger->info(name() + ": aes128-cbc-pkcs7 supported by system [OK]");
            m_key = QCA::SymmetricKey( QString(name()+"%/.?!:;]{[}&").toUtf8() );
        }
        else
        {
            m_logger->warning(name() + ": aes128-cbc-pkcs7 supported by system [KO]");
            QList<QCA::Provider*> prov = QCA::providers();
            m_logger->debug("> QCA2: " + QString::number(prov.size()) + " providers found");
            for(int i = 0; i<prov.size(); i++)
                m_logger->debug(prov.at(i)->name());

            m_logger->debug(QCA::pluginDiagnosticText());
        }

    }


    QString SECryptoAES::name() const
    {
        return QString("Silent Eye Encryption "+typeSupported());
    }

    QString SECryptoAES::version() const
    {
        return QString("1.1");
    }

    QString SECryptoAES::status()
    {
        init();
        if(m_isQcaCompatible)
            return "OK|aes128-cbc-pkcs7 supported by system";
        else
            return "KO|'aes128-cbc-pkcs7' not supported by system.\nPlease check your 'libqca2-plugin-ossl' installation.";
    }

    QString SECryptoAES::typeSupported() const
    {
        return QString("AES128");
    }

    QCA::SecureArray SECryptoAES::initializationVector(QString key)
    {
        QByteArray hash = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Md5);
        QString md5 = hash.toHex();
        QString value;
        for (int i=0; i<floor(128 / md5.size()); i++)
        {
            value += md5;
        }
        m_logger->debug("encrypted key: " + value);
        return QCA::InitializationVector( QCA::SecureArray(value.toUtf8()) );
    }

    QPointer<EncodedData> SECryptoAES::encode(QString key, QPointer<EncodedData> msg)
    {
        init();
        if(!m_isQcaCompatible){
            throw ModuleException("aes128-cbc-pkcs7 is not supported by the system.");
        }

        QCA::InitializationVector iv = initializationVector(key);

        // create a 128 bit AES cipher object using Cipher Block Chaining (CBC) mode
        QCA::Cipher cipher(QString("aes128"),QCA::Cipher::CBC,
                           // use Default padding, which is equivalent to PKCS7 for CBC
                           QCA::Cipher::DefaultPadding,
                           // this object will encrypt
                           QCA::Encode,
                           m_key, iv);

        // we use the cipher object to encrypt the argument we passed in
        // the result of that is returned - note that if there is less than
        // 1 block, then nothing will be returned - it is buffered
        // update() can be called as many times as required.
        QCA::SecureArray u = cipher.update( QCA::SecureArray(msg->bytes()) );

        if (!cipher.ok())
            throw ModuleException("An error occured during the encryption process.",
                                  "An error occured during the cipher update !");

        if(msg->format() != Data::FILE)
            m_logger->debug("AES128 non-final encryption of " + QCA::arrayToHex(msg->bytes()) + " is " + QCA::arrayToHex(u.toByteArray()) );

        // Because we are using PKCS7 padding, we need to output the final (padded) block
        // Note that we should always call final() even with no padding, to clean up
        QCA::SecureArray f = cipher.final();

        if (!cipher.ok())
            throw ModuleException("An error occured during the encryption process.",
                                  "An error occured during the finalization of the cipher !");

        return new EncodedData(u.append(f).toByteArray(), Data::BYTES, false);
    }

    QPointer<EncodedData> SECryptoAES::decode(QString key, QPointer<EncodedData> data)
    {
        init();
        if(!m_isQcaCompatible){
            throw ModuleException("aes128-cbc-pkcs7 is not supported by the system.");
        }

        QCA::InitializationVector iv = initializationVector(key);

        // create a 128 bit AES cipher object using Cipher Block Chaining (CBC) mode
        QCA::Cipher cipher(QString("aes128"),QCA::Cipher::CBC,
                           // use Default padding, which is equivalent to PKCS7 for CBC
                           QCA::Cipher::DefaultPadding,
                           // this object will encrypt
                           QCA::Decode,
                           m_key, iv);

        // take that cipher text, and decrypt it
        QCA::SecureArray plainText = cipher.update( QCA::SecureArray(data->toData()->data()) );

        // check if the update() call worked
        if (!cipher.ok())
            throw ModuleException("An error occured during the decryption process. (wrong password key?)",
                                  "An error occured during the cipher update !");

        // Again we need to call final(), to get the last block (with its padding removed)
        plainText += cipher.final();

        // check if the final() call worked
        if (!cipher.ok())
            throw ModuleException("An error occured during the decryption process. (wrong password key?)",
                                  "An error occured during the finalization of the cipher !");

        QPointer<EncodedData> result = new EncodedData(plainText.toByteArray(), Data::F_UNDEF, false);
        if(result->format() != Data::FILE)
            m_logger->debug("AES128 decryption of " + QCA::arrayToHex(data->toData()->data()) + " is "  + result->toString());

        return result;
    }

}

Q_EXPORT_PLUGIN2(secryptoaes, SECryptoAES::SECryptoAES)
