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
//  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#include "testconfig.h"
#include <config.h>

using namespace SilentEyeFramework;

void TestConfig::load_data()
{
    QTest::addColumn<QString>("filepath");
    QTest::addColumn<QString>("filename");
    QTest::addColumn<bool>("excepted");

    QTest::newRow("FileExist") << "./" << "good" << true;
    QTest::newRow("FileWrongFilename") << "./" << "good.conf" << false;
    QTest::newRow("FileWrongFilepath") << "/" << "good" << false;
}

void TestConfig::load()
{
    QFETCH(QString, filepath);
    QFETCH(QString, filename);
    QFETCH(bool, excepted);

    Config config(filepath, filename);

    QCOMPARE( config.isLoaded(), excepted );
}

void TestConfig::encrypt_data()
{
    QTest::addColumn<QString>("key");
    QTest::addColumn<bool>("encrypt");

    QTest::newRow("clear") << "clearmode" << false;
    QTest::newRow("encrypt") << "encryptmode" << true;
}

void TestConfig::encrypt()
{
    QFETCH(QString, key);
    QFETCH(bool, encrypt);

    Config config("./", "encrypt");

    QString value = "myvalue";
    config.set(key, value, encrypt);
    config.save();
    QVERIFY(config.get(key, encrypt) != config.get(key, !encrypt));
    QCOMPARE(value, config.get(key, encrypt));
}

QTEST_MAIN(TestConfig); 
