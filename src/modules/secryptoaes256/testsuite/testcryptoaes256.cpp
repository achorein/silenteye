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

#include "testcryptoaes256.h"

void TestCryptoAES256::encryption_data()
{
    loadModule();

    QTest::addColumn<QString>("encryptionKey");
    QTest::addColumn<QString>("decryptionKey");
    QTest::addColumn<QString>("text");
    QTest::addColumn<bool>("excepted");

    QTest::newRow("Correct decryption key") << "my key" << "my key" << "some text to encrypte !" << true;
    QTest::newRow("Wrong decryption key") << "my key" << "an other wrong key" << "some text to encrypte !" << false;
    QTest::newRow("No encode key") << "" << "my key" << "some text to encrypte !" << false;
    QTest::newRow("No decode key") << "my key" << "" << "some text to encrypte !" << false;
}

void TestCryptoAES256::encryption()
{
    QFETCH(QString, encryptionKey);
    QFETCH(QString, decryptionKey);
    QFETCH(QString, text);
    QFETCH(bool, excepted);

    if(m_module==NULL)
        QFAIL("Can't load a SECrypoAES module...");

    try
    {
        QPointer<EncodedData> data = m_module->encode(encryptionKey, new EncodedData(text, Data::UTF8, false));
        QString result = m_module->decode(decryptionKey, data)->toString(Data::UTF8);

        if( (result==text && !excepted) || (result!=text && excepted) )
        {
            qDebug() << "Input: " << text;
            qDebug() << "Result: " << result;
        }

        QCOMPARE( result==text, excepted );
    }
    catch(SilentEyeException e)
    {
        if(excepted)
        {
            qWarning() << e.message() <<  " : " << e.details();
            QFAIL("ModuleException");
        }
    }
}

void TestCryptoAES256::loadModule()
{
    m_module = NULL;
    /* Load Module */
    QDir pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("../../../../modules");

    /* Load all modules in modules directory */
    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if( plugin )
        {
            plugin = loader.instance();
            CryptoModuleInterface* moduleC = qobject_cast<CryptoModuleInterface*>(plugin);
            if( moduleC && moduleC->typeSupported().contains("AES256"))
            {
                m_module = moduleC;
            }
        }
    }
}

QTEST_MAIN(TestCryptoAES256);
