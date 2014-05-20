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

#ifndef SECryptoAES256_H_
#define SECryptoAES256_H_

#include <QtCore>
#include <QtCrypto>
#include <cryptomoduleinterface.h>
#include <logger.h>

#ifdef QT_STATICPLUGIN
    Q_IMPORT_PLUGIN(qca_ossl)
#endif

using namespace SilentEyeFramework;

//! SilentEye offical cryprographic module for support of encryption in AES 256.
namespace SECryptoAES256 {

    class SECryptoAES256 : public CryptoModuleInterface
    {
        Q_OBJECT;
        Q_INTERFACES(SilentEyeFramework::CryptoModuleInterface);

    private:
        QPointer<Logger> m_logger;
        bool m_isQcaCompatible;
        QCA::SymmetricKey m_key;
        QCA::Initializer* m_init;

    public:
        SECryptoAES256();
        ~SECryptoAES256();

        QString name() const;
        QString version() const;
        QString status();

        QString typeSupported() const ;
        QPointer<EncodedData> encode(QString key, QPointer<EncodedData> msg);
        QPointer<EncodedData> decode(QString key, QPointer<EncodedData> data);

    private:
	void init();
        QCA::SecureArray initializationVector(QString);
    };
}

#endif
