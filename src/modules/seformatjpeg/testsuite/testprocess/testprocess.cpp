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

#include "testprocess.h"

#include "../../ycbcr.h"
#include "../../groupedimage.h"

using namespace SEFormatJPEG;

void TestProcess::process_data()
{
    QTest::addColumn<QString>("imagePath");

    QTest::newRow("Normal process") << "./source.png";
    QTest::newRow("Normal process") << "./source2.png";
    QTest::newRow("Normal process") << "./source3.png";
    QTest::newRow("Normal process") << "./CCI20072010_00000.bmp";
}

void TestProcess::process()
{
    QFETCH(QString, imagePath);
    int k = 5;

    QImage image(imagePath);
    GroupedImage::compactImage(image, k);

    imagePath = imagePath.section('.', 0, -2)+".jpeg";

    QVERIFY(image.save(imagePath, "JPEG", 75));
    //delete img;

    image.load(imagePath);

    for (int y=0; y<image.height(); y++)
    {
        for (int x=0; x<image.width(); x++)
        {
            QRgb rgb = image.pixel(x, y);

            YCbCr ycbcr1(rgb);
            //YCbCr ycbcr2(qRed(rgb), qGreen(rgb), qBlue(rgb));

            QRgb rgb2 = ycbcr1.toRGB();
            /*qDebug() << QString::number(qRed(rgb)) + "!=" + QString::number(qRed(rgb2));
            qDebug() << QString::number(qGreen(rgb)) + "!=" + QString::number(qGreen(rgb2));
            qDebug() << QString::number(qBlue(rgb)) + "!=" + QString::number(qBlue(rgb2));
            qDebug() << ycbcr1.toString();*/
            QVERIFY(qRed(rgb) == qRed(rgb2));
            QVERIFY(qGreen(rgb) == qGreen(rgb2));
            QVERIFY(qBlue(rgb) == qBlue(rgb2));

            //QVERIFY(ycbcr1 == ycbcr2);
        }
    }

    GroupedImage gi(image, k);
    for (int y = 0; y < gi.height(); y++)
    {
        for (int x = 0; x < gi.width(); x++)
        {
            PixelGroup* pg = gi.pixelGroup(x, y);
            bool showGroup = pg->miv() < 0;

            for (int pgY = 0; pgY<PixelGroup::height(); pgY++)
            {
                for (int pgX = 0; pgX<PixelGroup::width(); pgX++)
                {
                    QPointer<YCbCr> ycbcr = pg->pixel(pgX, pgY);
                    if (!ycbcr.isNull()) {
                        int posX = (x*PixelGroup::width()) + pgX;
                        int posY = (y*PixelGroup::height()) + pgY;
                        if (posX >= gi.initialWidth() || posY >= gi.initialHeight())
                        {
                            QVERIFY2(ycbcr->y() == -1, "Out of bound != -1");
                        }
                        else
                        {
                            QRgb rgb = image.pixel(posX, posY);
                            QString debug = "imagePos: " + QString::number(posX) + ":" + QString::number(posY)
                                          + " | groupPos: " + QString::number(x) + ":" + QString::number(y)
                                          + " | pixelPos: " + QString::number(pgX) + ":" + QString::number(pgY)
                                          + " | imgColor: " + YCbCr(rgb).toString()
                                          + " | groupColor: " + ycbcr->toString();
                            QVERIFY2(rgb == ycbcr->toRGB(), debug.toStdString().c_str());
                            QVERIFY2(YCbCr(rgb, this) == *ycbcr, debug.toStdString().c_str());
                        }
                    }
                    if (showGroup) {
                        qDebug() << " pixelPos: " + QString::number(pgX) + ":" + QString::number(pgY)
                                      + " | color: " + ycbcr->toString();
                    }
                }
            }
            QString debug = "Wrong MIV! groupPos: " + QString::number(x) + ":" + QString::number(y)
                            + " | MIV: " + QString::number(pg->miv());
            QVERIFY2(!showGroup, debug.toStdString().c_str());
        }
    }
    QImage* image2 = gi.toImage();

    QVERIFY2(image.width() == image2->width(), "Source and result doesn't have same width");
    QVERIFY2(image.height() == image2->height(), "Source and result doesn't have same height");
    for (int y=0; y<image.height(); y++) {
        for (int x=0; x<image.width(); x++) {
            QVERIFY2(image.pixel(x, y) == image2->pixel(x, y), "Source and result doesn't match");
        }
    }

    QVERIFY(image2->save(imagePath, "JPEG", 75));
    delete image2;

    image.load(imagePath);

    GroupedImage gi2(image, k);

    for (int y = 0; y < gi2.height(); y++)
    {
        for (int x = 0; x < gi2.width(); x++)
        {
            PixelGroup* pg = gi.pixelGroup(x, y);
            PixelGroup* pg2 = gi2.pixelGroup(x, y);
            QVERIFY2(pg->miv() == pg2->miv(), "Source and result doesn't match");
        }
    }
}

QTEST_APPLESS_MAIN(TestProcess);
