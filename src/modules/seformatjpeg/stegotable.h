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

#include <QString>
#include <QBitArray>
#include <QVector>

#include <logger.h>
#include <QPointer>

#ifndef STEGOTABLE_H_
#define STEGOTABLE_H_

#define DEFAULT_K 20

using namespace SilentEyeFramework;

namespace SEFormatJPEG
{
    //! Manage the K stegano tables and the index table.
    class StegoTable : QObject
    {
    private:
        //! Logger
        QPointer<Logger> m_logger;

        //! Intensity interval and number of stego-tables
        quint8 _k;

        //! Current stegano table used
        quint8 _currentTable;

        //! stego tables
        QVector<QBitArray> _stTables;

        //! extra index table
        QVector<quint8> _indexTable;

        //! MD5 generated from the passphrase (required for table generation)
        QString _md5;

    public:
        //! Generate the Stego Table according to the interval length and the passphrase.
        StegoTable(const QString&, const quint8=DEFAULT_K, QObject* parent=0);
        ~StegoTable();

        //! Intensity interval length value
        quint8 k();

        float computeNewMiv(float, bool);
        bool computeValue(float, bool=true);

        //! Compute a String representing all the stegano tables and the index table
        QString toString();

    private:
        //! BitArray table reprensenting current Stego table
        QBitArray getNextTable();
        //! Compute middle of an interval, depending on an intensity value
        float computeMiddle(float);
        //! Generate the Stego according to the md5 generated and the intensity interval.
        void init_tables();
        //! Initialize the index table which determine which stego-table is going to be use.
        void initIndexTable();

    };
}

#endif
