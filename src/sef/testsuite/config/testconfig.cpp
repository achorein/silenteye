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

  m_config = new Config(filepath, filename);

  QCOMPARE( m_config->isLoaded(), excepted );
}

QTEST_MAIN(TestConfig); 
