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

#include "testformatwav.h"

#include <QPointer>
#include "../audiowav.h"

using namespace SEFormatWAV;

void TestFormatWAV::process_data()
{
    loadModule();

    QTest::addColumn<QString>("soundPath");
    QTest::addColumn<QString>("text");

    QTest::newRow("Normal process") << "./source-16b-stereo.wav" << "My pretty text to encode";
}

void TestFormatWAV::process()
{
    QFETCH(QString, soundPath);
    QFETCH(QString, text);

    if(m_module==NULL)
        QFAIL("Can't load a SEFormatWAV module...");

    QString dest("./");
    try
    {
        QPointer<Audio> audio = new Audio(soundPath);
        QPointer<Audio> wav = m_module->encodeAudio(audio);
        QPointer<EncodedData> data = new EncodedData(text, Data::UTF8, false);
        wav->setEncodedData(data);
        QCOMPARE(wav->encodedData()->toString(), text);
        QVERIFY(wav->saveToDir(dest));
        qDebug() << "Compilation finished.";
        QVERIFY(wav->loadData());
        qDebug() << "Load finished.";
        QVERIFY(wav->isDataLoaded());
        QVERIFY(!(wav->encodedData().isNull()));
        QCOMPARE(wav->encodedData()->size(), data->size());
        QCOMPARE(wav->encodedData()->toString(), text);
    }
    catch(SilentEyeException e)
    {
        qWarning() << e.message() <<  " : " << e.details();
        QFAIL("ModuleException");
    }
}

void TestFormatWAV::manualProcess_data()
{
    QTest::addColumn<QString>("soundPath");
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("header");
    QTest::addColumn<int>("distribution");

    // 16 bit 2 channels
    QTest::newRow("Manual process (16-2-BI)") << "./source-16b-stereo.wav" << "My pretty text to encode"
            << (int)AudioWav::BEGINNING << (int)AudioWav::INLINE;
    QTest::newRow("Manual process (16-2-BE)") << "./source-16b-stereo.wav" << "My pretty text to encode"
            << (int)AudioWav::BEGINNING << (int)AudioWav::EQUI;
    QTest::newRow("Manual process (16-2-EI)") << "./source-16b-stereo.wav" << "My pretty text to encode"
            << (int)AudioWav::ENDING << (int)AudioWav::INLINE;
    QTest::newRow("Manual process (16-2-EE)") << "./source-16b-stereo.wav" << "My pretty text to encode"
            << (int)AudioWav::ENDING << (int)AudioWav::EQUI;

    /*
     *  16 bit 1 channel
     */
    QTest::newRow("Manual process (16-1-BI)") << "./source-16b-mono.wav" << "My pretty text to encode"
            << (int)AudioWav::BEGINNING << (int)AudioWav::INLINE;
    QTest::newRow("Manual process (16-1-BE)") << "./source-16b-mono.wav" << "My pretty text to encode"
            << (int)AudioWav::BEGINNING << (int)AudioWav::EQUI;
    QTest::newRow("Manual process (16-1-EI)") << "./source-16b-mono.wav" << "My pretty text to encode"
            << (int)AudioWav::ENDING << (int)AudioWav::INLINE;
    QTest::newRow("Manual process (16-1-EE)") << "./source-16b-mono.wav" << "My pretty text to encode"
            << (int)AudioWav::ENDING << (int)AudioWav::EQUI;


    /*
     * 8 bit 2 channels
     */
    QTest::newRow("Manual process (8-2-BI)") << "./source-8b-stereo.wav" << "My pretty text to encode"
            << (int)AudioWav::BEGINNING << (int)AudioWav::INLINE;
    QTest::newRow("Manual process (8-2-BE)") << "./source-8b-stereo.wav" << "My pretty text to encode"
            << (int)AudioWav::BEGINNING << (int)AudioWav::EQUI;
    QTest::newRow("Manual process (8-2-EI)") << "./source-8b-stereo.wav" << "My pretty text to encode"
            << (int)AudioWav::ENDING << (int)AudioWav::INLINE;
    QTest::newRow("Manual process (8-2-EE)") << "./source-8b-stereo.wav" << "My pretty text to encode"
            << (int)AudioWav::ENDING << (int)AudioWav::EQUI;


    /*
     * 8 bit 1 channel
     */
    QTest::newRow("Manual process (8-1-BI)") << "./source-8b-mono.wav" << "My pretty text to encode"
            << (int)AudioWav::BEGINNING << (int)AudioWav::INLINE;
    QTest::newRow("Manual process (8-1-BE)") << "./source-8b-mono.wav" << "My pretty text to encode"
            << (int)AudioWav::BEGINNING << (int)AudioWav::EQUI;
    QTest::newRow("Manual process (8-1-EI)") << "./source-8b-mono.wav" << "My pretty text to encode"
            << (int)AudioWav::ENDING << (int)AudioWav::INLINE;
    QTest::newRow("Manual process (8-1-EE)") << "./source-8b-mono.wav" << "My pretty text to encode"
            << (int)AudioWav::ENDING << (int)AudioWav::EQUI;

}

void TestFormatWAV::manualProcess()
{
    QFETCH(QString, soundPath);
    QFETCH(QString, text);
    QFETCH(int, header);
    QFETCH(int, distribution);

    QString dest("./");


    for (int bits = 1; bits <= 6; bits++)
    {
        AudioWav* wav = NULL;
        try {
            wav = new AudioWav(soundPath);
        }
        catch(SilentEyeException e)
        {
            qWarning() << e.message() <<  " : " << e.details();
            QFAIL("ModuleException");
        }

        try
        {
            QPointer<EncodedData> data = new EncodedData(text, Data::UTF8, false);
            wav->setNbBitsUsed(bits);
            wav->setNbChannelUsed(wav->format().channels());
            wav->setHeaderPosition((AudioWav::HeaderPosition) header);
            wav->setDistribution((AudioWav::DataDistribution) distribution);
            wav->setEncodedData(data);
            QVERIFY(wav->saveToDir(dest));
            qDebug() << "Compilation finished. (" + soundPath + ")";
            QVERIFY(wav->loadData());
            qDebug() << "Load finished. (" + soundPath + ")";
            QVERIFY(wav->isDataLoaded());
            QVERIFY(!(wav->encodedData().isNull()));
            QCOMPARE(wav->encodedData()->size(), data->size());
            QCOMPARE(wav->encodedData()->toString(), text);
        }
        catch(SilentEyeException e)
        {
            qWarning() << e.message() <<  " : " << e.details();
            if (wav != NULL)
                delete wav;
            QFAIL("ModuleException");
        }
        if (wav != NULL)
            delete wav;
    }
}

void TestFormatWAV::loadModule()
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
            AudioModuleInterface* moduleF = qobject_cast<AudioModuleInterface*>(plugin);
            if( moduleF && moduleF->typeSupported().contains("WAV"))
            {
                m_module = moduleF;
		break;
            }
        }
    }
}

QTEST_MAIN(TestFormatWAV);
