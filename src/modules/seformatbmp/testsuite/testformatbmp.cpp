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

#include "testformatbmp.h"

#include <QPointer>
#include "../imagebmp.h"

using namespace SEFormatBMP;

void TestFormatBMP::process_data()
{
    loadModule();

    QTest::addColumn<QString>("imagePath");
    QTest::addColumn<QString>("text");

    QTest::newRow("Normal process") << "./source.png" << "My pretty text to encode";
    QTest::newRow("Normal process") << "./source2.png" << "My pretty text to encode";
    QTest::newRow("Normal process") << "./source3.png" << "My pretty text to encode";
}

void TestFormatBMP::process()
{
    QFETCH(QString, imagePath);
    QFETCH(QString, text);

    if(m_module==NULL)
        QFAIL("Can't load a SEFormatBMP module...");

    QString dest("./");
    try
    {
        QPointer<Image> img = new Image(imagePath);
        QVERIFY(!(img->isNull()));
        QPointer<Image> imgBmp = m_module->encodeImage(img);
        QPointer<EncodedData> data = new EncodedData(text, Data::UTF8, false);
        imgBmp->setEncodedData(data);
        QCOMPARE(imgBmp->encodedData()->toString(), text);
        QVERIFY(imgBmp->saveToDir(dest));
        qDebug() << "Compilation finished.";
        QVERIFY(imgBmp->loadData());
        qDebug() << "Load finished.";
        QVERIFY(imgBmp->isDataLoaded());
        QVERIFY(!(imgBmp->encodedData().isNull()));
        QCOMPARE(imgBmp->encodedData()->size(), data->size());
        QCOMPARE(imgBmp->encodedData()->toString(), text);
    }
    catch(SilentEyeException e)
    {
        qWarning() << e.message() <<  " : " << e.details();
        QFAIL("ModuleException");
    }
}

void TestFormatBMP::manualProcess_data()
{
    QTest::addColumn<QString>("imagePath");
    QTest::addColumn<QString>("text");
    QTest::addColumn<bool>("useRed");
    QTest::addColumn<bool>("useGreen");
    QTest::addColumn<bool>("useBlue");
    QTest::addColumn<int>("header");
    QTest::addColumn<int>("distribution");


    QTest::newRow("Manual process (all color)") << "./source.png" << "My pretty text to encode" 
            << true << true << true
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (red)") << "./source.png" << "My pretty text to encode" 
            << true << false << false
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (green)") << "./source.png" << "My pretty text to encode" 
            << false << true << false
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (blue)") << "./source.png" << "My pretty text to encode" 
            << false << false << true
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (red, green)") << "./source.png" << "My pretty text to encode"
            << true << true << false
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (red, blue)") << "./source.png" << "My pretty text to encode" 
            << true << false << true
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (green, blue)") << "./source.png" << "My pretty text to encode" 
            << false << true << true
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (bottom, inline)") << "./source.png" << "My pretty text to encode" 
            << false << true << true
            << (int)ImageBMP::BOTTOM << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (signature, inline)") << "./source.png" << "My pretty text to encode" 
            << false << true << true
            << (int)ImageBMP::SIGNATURE << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (signature, equi)") << "./source.png" << "My pretty text to encode" 
            << false << true << true
            << (int)ImageBMP::SIGNATURE << (int)ImageBMP::EQUI;
    QTest::newRow("Manual process (signature, equi, small)") << "./source.png" << "test"
            << true << true << true
            << (int)ImageBMP::SIGNATURE << (int)ImageBMP::EQUI;
    QTest::newRow("Manual process (signature, equi, verysmall)") << "./source.png" << "up"
            << true << true << true
            << (int)ImageBMP::SIGNATURE << (int)ImageBMP::EQUI;
    QTest::newRow("Manual process (signature, equi, long)") << "./source.png"
            << QString("My pretty text to encode My pretty text to encode My pretty text to encode My pretty text to encode ")
                + "My pretty text to encode My pretty text to encode My pretty text to encode My pretty text to encode "
                + "My pretty text to encode My pretty text to encode My pretty text to encode My pretty text to encode "
                + "My pretty text to encode My pretty text to encode My pretty text to encode My pretty text to encode "
                + "My pretty text to encode My pretty text to encode"
            << false << false << true
            << (int)ImageBMP::SIGNATURE << (int)ImageBMP::EQUI;
    QTest::newRow("Manual process (top, equi)") << "./source.png" << "My pretty text to encode" 
            << true << true << true
            << (int)ImageBMP::TOP << (int)ImageBMP::EQUI;
    QTest::newRow("Manual process (bottom, equi)") << "./source.png" << "My pretty text to encode" 
            << true << true << true
            << (int)ImageBMP::BOTTOM << (int)ImageBMP::EQUI;

    QTest::newRow("Manual process (all color)") << "./source2.png" << "My pretty text to encode"
            << true << true << true
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (red)") << "./source2.png" << "My pretty text to encode"
            << true << false << false
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (green)") << "./source2.png" << "My pretty text to encode"
            << false << true << false
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (blue)") << "./source2.png" << "My pretty text to encode"
            << false << false << true
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (red, green)") << "./source2.png" << "My pretty text to encode"
            << true << true << false
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (red, blue)") << "./source2.png" << "My pretty text to encode"
            << true << false << true
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (green, blue)") << "./source2.png" << "My pretty text to encode"
            << false << true << true
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (bottom, inline)") << "./source2.png" << "My pretty text to encode"
            << false << true << true
            << (int)ImageBMP::BOTTOM << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (signature, inline)") << "./source2.png" << "My pretty text to encode"
            << false << true << true
            << (int)ImageBMP::SIGNATURE << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (signature, equi)") << "./source2.png" << "My pretty text to encode"
            << false << true << true
            << (int)ImageBMP::SIGNATURE << (int)ImageBMP::EQUI;
    QTest::newRow("Manual process (signature, equi, small)") << "./source2.png" << "test"
            << true << true << true
            << (int)ImageBMP::SIGNATURE << (int)ImageBMP::EQUI;
    QTest::newRow("Manual process (signature, equi, verysmall)") << "./source2.png" << "up"
            << true << true << true
            << (int)ImageBMP::SIGNATURE << (int)ImageBMP::EQUI;
    QTest::newRow("Manual process (top, equi)") << "./source2.png" << "My pretty text to encode"
            << true << true << true
            << (int)ImageBMP::TOP << (int)ImageBMP::EQUI;
    QTest::newRow("Manual process (bottom, equi)") << "./source2.png" << "My pretty text to encode"
            << true << true << true
            << (int)ImageBMP::BOTTOM << (int)ImageBMP::EQUI;

    QTest::newRow("Manual process (all color)") << "./source3.png" << "My pretty text to encode"
            << true << true << true
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (red)") << "./source3.png" << "My pretty text to encode"
            << true << false << false
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (green)") << "./source3.png" << "My pretty text to encode"
            << false << true << false
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (blue)") << "./source3.png" << "My pretty text to encode"
            << false << false << true
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (red, green)") << "./source3.png" << "My pretty text to encode"
            << true << true << false
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (red, blue)") << "./source3.png" << "My pretty text to encode"
            << true << false << true
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (green, blue)") << "./source3.png" << "My pretty text to encode"
            << false << true << true
            << (int)ImageBMP::TOP << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (bottom, inline)") << "./source3.png" << "My pretty text to encode"
            << false << true << true
            << (int)ImageBMP::BOTTOM << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (signature, inline)") << "./source3.png" << "My pretty text to encode"
            << false << true << true
            << (int)ImageBMP::SIGNATURE << (int)ImageBMP::INLINE;
    QTest::newRow("Manual process (signature, equi)") << "./source3.png" << "My pretty text to encode"
            << false << true << true
            << (int)ImageBMP::SIGNATURE << (int)ImageBMP::EQUI;
    QTest::newRow("Manual process (signature, equi, small)") << "./source3.png" << "test"
            << true << true << true
            << (int)ImageBMP::SIGNATURE << (int)ImageBMP::EQUI;
    QTest::newRow("Manual process (signature, equi, verysmall)") << "./source3.png" << "up"
            << true << true << true
            << (int)ImageBMP::SIGNATURE << (int)ImageBMP::EQUI;
    QTest::newRow("Manual process (signature, equi, long)") << "./source.png"
            << QString("My pretty text to encode My pretty text to encode My pretty text to encode My pretty text to encode ")
                + "My pretty text to encode My pretty text to encode My pretty text to encode My pretty text to encode "
                + "My pretty text to encode My pretty text to encode My pretty text to encode My pretty text to encode "
                + "My pretty text to encode My pretty text to encode My pretty text to encode My pretty text to encode "
                + "My pretty text to encode My pretty text to encode"
            << false << false << true
            << (int)ImageBMP::SIGNATURE << (int)ImageBMP::EQUI;
    QTest::newRow("Manual process (top, equi)") << "./source3.png" << "My pretty text to encode"
            << true << true << true
            << (int)ImageBMP::TOP << (int)ImageBMP::EQUI;
    QTest::newRow("Manual process (bottom, equi)") << "./source3.png" << "My pretty text to encode"
            << true << true << true
            << (int)ImageBMP::BOTTOM << (int)ImageBMP::EQUI;
}

void TestFormatBMP::manualProcess()
{
    QFETCH(QString, imagePath);
    QFETCH(QString, text);
    QFETCH(bool, useRed);
    QFETCH(bool, useGreen);
    QFETCH(bool, useBlue);
    QFETCH(int, header);
    QFETCH(int, distribution);

    QString dest("./");
    for (int bits = 1; bits <= 6; bits++)
    {
        ImageBMP* img = new ImageBMP(imagePath);
        try
        {
            QVERIFY(!(img->isNull()));
            QPointer<EncodedData> data = new EncodedData(text, Data::UTF8, false);
            img->setNbBits(2);
            img->setColorUsed(useRed, useGreen, useBlue);
            img->setHeaderPosition((ImageBMP::HeaderPosition) header);
            img->setDistribution((ImageBMP::DataDistribution) distribution);
            img->setEncodedData(data);
            QVERIFY(img->saveToDir(dest));
            qDebug() << "Compilation finished. (" + imagePath + ")";
            QVERIFY(img->loadData());
            qDebug() << "Load finished. (" + imagePath + ")";
            QVERIFY(img->isDataLoaded());
            QVERIFY(!(img->encodedData().isNull()));
            QCOMPARE(img->encodedData()->size(), data->size());
            QCOMPARE(img->encodedData()->toString(), text);
        }
        catch(SilentEyeException e)
        {
            qWarning() << e.message() <<  " : " << e.details();
            QFAIL("ModuleException");
        }
        delete img;
    }
}

void TestFormatBMP::loadModule()
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
            ImageModuleInterface* moduleF = qobject_cast<ImageModuleInterface*>(plugin);
            if( moduleF && moduleF->typeSupported().contains("BMP"))
            {
                m_module = moduleF;
		break;
            }
        }
    }
}

QTEST_MAIN(TestFormatBMP); 
