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

#include "testformatjpeg.h"

#include <QPointer>
#include "../../imagejpeg.h"
#include  <logger.h>

using namespace SEFormatJPEG;

void TestFormatJPEG::process_data()
{
    Logger::setLevel(Logger::INFO_LEVEL);
    loadModule();

    QTest::addColumn<QString>("imagePath");
    QTest::addColumn<QString>("text");

    QTest::newRow("Normal process1") << "./source1.png" << "My pretty text to encode";
    QTest::newRow("Normal process2") << "./source2.png" << "My pretty text to encode";
    QTest::newRow("Normal process3") << "./source3.png" << "My pretty text to encode into JPEG";
    QTest::newRow("Normal process4") << "./source4.png" << "My pretty text!";
}

void TestFormatJPEG::process()
{
    QFETCH(QString, imagePath);
    QFETCH(QString, text);

    if(m_module==NULL)
        QFAIL("Can't load a SEFormatJPEG module...");

    QString dest("./");
    try
    {
        QPointer<Image> img = new Image(imagePath);
        QVERIFY(!(img->isNull()));
        QPointer<Image> imgJPEG = m_module->encodeImage(img);
        QPointer<EncodedData> data = new EncodedData(text, Data::UTF8, false);
        imgJPEG->setEncodedData(data);
        QCOMPARE(imgJPEG->encodedData()->toString(), text);
        QVERIFY(imgJPEG->saveToDir(dest));
        qDebug() << "Compilation finished.";
        QVERIFY(imgJPEG->loadData());
        qDebug() << "Load finished.";
        QVERIFY(imgJPEG->isDataLoaded());
        QVERIFY(!(imgJPEG->encodedData().isNull()));
        QCOMPARE(imgJPEG->encodedData()->size(), data->size());
        QCOMPARE(imgJPEG->encodedData()->toString(), text);
    }
    catch(SilentEyeException e)
    {
        qWarning() << e.message() <<  " : " << e.details();
        QFAIL("SilentEyeException");
    }
    catch(ModuleException* e)
    {
        qWarning() << e->message() <<  " : " << e->details();
        QFAIL("ModuleException");
    }
    catch(QtConcurrent::Exception e)
    {
        qWarning() << e.what();
        QFAIL("Unknown Exception");
    }
}

void TestFormatJPEG::manualProcess_data()
{
    QTest::addColumn<QString>("imagePath");
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("header");

    QTest::newRow("Manual process (top1)") << "./source1.png" << "My pretty text to encode"
            << (int)ImageJPEG::TOP;
    QTest::newRow("Manual process (bottom1)") << "./source1.png" << "My pretty text to encode"
            << (int)ImageJPEG::BOTTOM;
    QTest::newRow("Manual process (bottom1, size max)") << "./source1.png"
            << QString("My pretty text to encod->27")
            << (int)ImageJPEG::BOTTOM;

    QTest::newRow("Manual process (top2)") << "./source2.png" << "My pretty text to encode"
            << (int)ImageJPEG::TOP;
    QTest::newRow("Manual process (bottom2)") << "./source2.png" << "My pretty text to encode"
            << (int)ImageJPEG::BOTTOM;
    QTest::newRow("Manual process (bottom2, size max)") << "./source2.png"
            << QString("My pretty text to encod->27")
            << (int)ImageJPEG::TOP;


    QTest::newRow("Manual process (top3)") << "./source3.png" << "My pretty text to encode"
            << (int)ImageJPEG::TOP;
    QTest::newRow("Manual process (bottom3)") << "./source3.png" << "My pretty text to encode"
            << (int)ImageJPEG::BOTTOM;
    QTest::newRow("Manual process (signature3)") << "./source3.png" << "My pretty text to encode"
            << (int)ImageJPEG::SIGNATURE;
    QTest::newRow("Manual process (signature3, small)") << "./source3.png" << "test"
            << (int)ImageJPEG::SIGNATURE;
    QTest::newRow("Manual process (signature3, verysmall)") << "./source3.png" << "up"
            << (int)ImageJPEG::SIGNATURE;
    QTest::newRow("Manual process (bottom3, size max)") << "./source3.png"
            << QString("My pretty text to encode My pretty text to encode ")
            + "My pretty text to encode My pretty text to encode "
            + "My pretty text to encode My pretty text ->146"
            << (int)ImageJPEG::BOTTOM;
            
    QTest::newRow("Manual process (top4)") << "./source4.png" << "Myprettytex"
            << (int)ImageJPEG::TOP;
    QTest::newRow("Manual process (bottom4)") << "./source4.png" << "Myprettytexe"
            << (int)ImageJPEG::BOTTOM;
    QTest::newRow("Manual process (bottom4, size max)") << "./source3.png"
            << QString("Myprettytext->16")
            << (int)ImageJPEG::BOTTOM;
}

void TestFormatJPEG::manualProcess()
{
    QFETCH(QString, imagePath);
    QFETCH(QString, text);
    QFETCH(int, header);

    if(m_module==NULL)
        QFAIL("Can't load a SEFormatJPEG module...");
    
    QString dest("./");
    for (int k = 20; k >= 5; k-=3)
    {
        ImageJPEG* img = new ImageJPEG(imagePath);
        try
        {
            QVERIFY(!(img->isNull()));
            QPointer<EncodedData> data = new EncodedData(text, Data::UTF8, false);
            img->setHeaderPosition((ImageJPEG::HeaderPosition) header);
            img->setEncodedData(data);
            img->setK(k);
            QVERIFY(img->saveToDir(dest));
            qDebug() << "Compilation finished. (" + imagePath + ", " + QString::number(k) + ") ";
            QVERIFY(img->loadData());
            qDebug() << "Load finished. (" + imagePath + ", " + QString::number(k) + ") ";
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

void TestFormatJPEG::loadModule()
{
    m_module = NULL;

    /* Load Module */
    QDir pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("../../../../../modules");

    /* Load all modules in modules directory */
    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if( plugin )
        {
            ImageModuleInterface* moduleF = qobject_cast<ImageModuleInterface*>(plugin);
            if( moduleF && moduleF->typeSupported().contains("JPEG"))
            {
                m_module = moduleF;
		break;
            }
        }
    }
}

QTEST_MAIN(TestFormatJPEG); 
