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

#include "testencodeddata.h"
#include "encodeddata.h"
#include "silenteyeexception.h"

void TestEncodedData::load_data()
{
    QTest::addColumn<QString>("value");
    QTest::addColumn<bool>("compressed");

    QTest::newRow("Normal") << "My text to encode My text to encode My text to encode My text to encode " << false;
    QTest::newRow("Compressed") << "My text to encode My text to encode My text to encode My text to encode " << true;
}

void TestEncodedData::load()
{
    QFETCH(QString, value);
    QFETCH(bool, compressed);

    /*if (compressed)
    {
        QByteArray original = value.toUtf8();
        qDebug() << "original size: " << original.size();
        QByteArray compressed = qCompress(original, 9);
        qDebug() << "compressed size: " << compressed.size();
        QByteArray uncompressed = qUncompress(compressed);
        qDebug() << "manualy: " << QString::fromUtf8(uncompressed.data());
    }*/
    try
    {
        EncodedData data1(value, Data::UTF8, compressed);
        QCOMPARE(data1.toString(), value);
        QCOMPARE(QString::fromUtf8(data1.toData()->data()), value);

        EncodedData data2(value, Data::LATIN1, compressed);
        QCOMPARE(data2.toString(), value);
        QCOMPARE(QString::fromUtf8(data2.toData()->data()), value);

        EncodedData data3(value, Data::ASCII, compressed);
        QCOMPARE(data3.toString(), value);
        QCOMPARE(QString::fromUtf8(data3.toData()->data()), value);
    }
    catch(SilentEyeException e)
    {
        qWarning() << e.message() <<  " : " << e.details();
        QFAIL("ModuleException");
    }
}

void TestEncodedData::read_data()
{
    QTest::addColumn<QString>("value");
    QTest::addColumn<bool>("compressed");

    QTest::newRow("Normal") << "My text to encode My text to encode My text to encode My text to encode " << false;
    QTest::newRow("Normal UTF8") << "My téxt to êncode My tèxt to €ncode My téxt to êncode My tèxt to €ncode\n" << false;
    QTest::newRow("Compressed") << "My text to encode My text to encode My text to encode My text to encode " << true;
    QTest::newRow("Compressed UTF8") << "My téxt to êncode My tèxt to €ncode My téxt to êncode My tèxt to €ncode\n" << true;
}

void TestEncodedData::read()
{
    QFETCH(QString, value);
    QFETCH(bool, compressed);

    EncodedData dataSource(value, Data::UTF8, compressed);

    try
    {
        for (int nbBits=1; nbBits<=8; nbBits++) {
            qDebug() << "Read " << nbBits << " bits";
            EncodedData dataDest(Data::F_UNDEF, compressed);

            dataSource.initialize(nbBits); // number of bit read in one shot
            dataDest.initialize(nbBits);
            while(dataSource.hasNext())
            {
                int val = dataSource.read();
                dataDest.append(val);
            }
            QCOMPARE(dataDest.toString(), value);
        }
    }
    catch(SilentEyeException e)
    {
        qWarning() << e.message() <<  " : " << e.details();
        QFAIL("ModuleException");
    }

}

void TestEncodedData::compress_data()
{
    QTest::addColumn<QString>("value");

    QTest::newRow("Normal") << "My text to encode My text to encode My text to encode My text to encode ";
}

void TestEncodedData::compress()
{
    QFETCH(QString, value);

    try
    {
        EncodedData notCompressed(value, Data::UTF8, false);
        EncodedData compressed(value, Data::UTF8, true);

        QVERIFY(notCompressed.size() > compressed.size());
        QCOMPARE(notCompressed.toString(), value);
        QCOMPARE(compressed.toString(), value);
    }
    catch(SilentEyeException e)
    {
        qWarning() << e.message() <<  " : " << e.details();
        QFAIL("ModuleException");
    }
}

void TestEncodedData::encodeFile_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<bool>("compress");

    QTest::newRow("Normal") << "CMakeLists.txt" << false;
    QTest::newRow("Compressed") << "CMakeLists.txt" << true;
}

void TestEncodedData::encodeFile()
{
    QFETCH(QString, fileName);
    QFETCH(bool, compress);

    try
    {
        QVERIFY(QFile::exists(fileName));
        QFile file(fileName);
        EncodedData dataSource(file, compress);
        QPointer<Data> dataSrc = dataSource.toData();

        EncodedData dataDest;


        dataSource.initialize(3);
        dataDest.initialize(3);
        while(dataSource.hasNext())
        {
            int val = dataSource.read();
            dataDest.append(val);
        }
        dataDest.setCompressed(compress);
        QPointer<Data> data = dataDest.toData();
        QCOMPARE(data->format(), Data::FILE);
        QCOMPARE(data->name(), fileName);
        QCOMPARE(data->data().size(), dataSrc->data().size());
        QFile out("output.txt");
        out.open(QFile::WriteOnly);
        out.write(data->data());
        out.flush();
        out.close();
    }
    catch(SilentEyeException e)
    {
        qWarning() << e.message() <<  " : " << e.details();
        QFAIL("ModuleException");
    }
}

QTEST_MAIN(TestEncodedData);
