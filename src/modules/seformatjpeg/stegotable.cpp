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

#include "stegotable.h"

#include <moduleexception.h>

#include <math.h>

using namespace SilentEyeFramework;

namespace SEFormatJPEG
{
    //! Generate the Stego Table according to the interval length and the passphrase.
    StegoTable::StegoTable(const QString& passphrase, const quint8 k, QObject* parent) : QObject(parent), _k(k)
    {
        setObjectName("StegoTable");
        m_logger = new Logger(this);

	QString key = "SilentEye";
        if (passphrase.trimmed() != "")
        {
           key = passphrase.trimmed();
        }

	QByteArray hash = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Md5);
	_md5 = hash.toHex();

        _currentTable = 0;
        init_tables();
    }

    StegoTable::~StegoTable()
    {
        // NOTHING TO DO
    }

    //! Calculate the begining of an interval, depending on an intensity value
    float StegoTable::computeMiddle(float miv)
    {
        qint16 inf = floor(miv / (double)_k) * _k;
        float val = inf + (_k / 2.0);
        /*m_logger->debug("miv: " + QString::number(val)
                        + "=> stegotable index: " + QString::number( floor(val / (double)_k)));*/
        return val;
    }

    //! Intensity interval length value
    quint8 StegoTable::k()
    {
        return _k;
    }

    float StegoTable::computeNewMiv(float oldMiv, bool value)
    {
        float newMiv = -1;
        getNextTable();

        if (computeValue(oldMiv, false) !=  value)
        {
            for (int i = 1; i<3; i++)
            {
                newMiv = oldMiv - (i*_k);
                if (newMiv >= 0)
                {
                    if (computeValue(newMiv, false) == value)
                    {
                        newMiv = computeMiddle(newMiv);
                        if (newMiv > 255)
                        {
                            newMiv = 255;
                        }
                        if (computeValue(newMiv, false) != value)
                        {
                            throw ModuleException("Invalid new miv value!", QString::number(newMiv));
                        }
                        return newMiv;
                    }
                }

                newMiv = oldMiv + (i*_k);
                if (newMiv <= 255)
                {
                    if (computeValue(newMiv, false) == value)
                    {
                        newMiv = computeMiddle(newMiv);
                        if (newMiv > 255)
                        {
                            newMiv = 255;
                        }
                        if (computeValue(newMiv, false) != value)
                        {
                            throw ModuleException("Invalid new miv value!", QString::number(newMiv));
                        }
                        return newMiv;
                    }
                }
            }

            throw ModuleException("Cannot find interval in stgeoTable", "MIV : " + QString::number(oldMiv));

        } else {
            newMiv = computeMiddle(oldMiv);
            if (computeValue(newMiv, false) != value)
            {
                throw ModuleException("Invalid new miv value!", QString::number(newMiv));
            }
        }

        return newMiv;
    }

    //! Compute hidden value according to stegotable
    bool StegoTable::computeValue(float miv, bool next)
    {
        QBitArray stegoTable = next ? getNextTable() : _stTables[_indexTable[_currentTable]];
        int index = floor(miv / (double)_k);
        /*m_logger->debug("miv: " + QString::number(miv)
                        + "=> stegotable index: " + QString::number(index));*/
        return stegoTable.at(index);
    }

    //! BitArray table reprensenting the Stego tables
    QBitArray StegoTable::getNextTable()
    {
        _currentTable++;
        if (_currentTable >= _indexTable.size())
        {
            _currentTable = 0;
        }
        return _stTables[_indexTable[_currentTable]];
    }

    //! Generate the Stego according to the md5 generated and the intensity interval.
    void StegoTable::init_tables()
    {
        int currentMD5position = 0; // using char index of the md5 string

        // Converts the first character of md5 into int
        int currentMD5value = -48 + _md5[currentMD5position].digitValue();

        int stSize = (int)ceil(255 / (double)_k); // stego table length
        stSize = stSize % 2 == 0 ? stSize : stSize + 1;

        _stTables.clear();
        _stTables.resize(_k);
        for (int stNumber = 0; stNumber < _k; stNumber++)
        {
            _stTables[stNumber] = QBitArray(stSize);
            // nbBitInsered: number of bits insered into the current stego table
            int nbBitInsered = 0; bool bit = false;
            do
            {
                // compute and add the next bit into the current stego stable
                if (nbBitInsered < stSize)
                {
                    // get the bit to set into the table (&1 logical and operator, likes subnet mask)
                    bit = ((currentMD5value & 1) == 1);
                    //Add the curent bit into the stego table
                    _stTables[stNumber][nbBitInsered++] = bit;
                }
                // force next bit for no continious bits like 000 or 111 in a table.
                if (nbBitInsered != 0 && nbBitInsered < stSize)
                {
                    _stTables[stNumber][nbBitInsered++] = !bit;
                }

                // update the current md5 char (move index or update bits)
                if ((stNumber * stSize + nbBitInsered) % 5 != 0) // %5 because a char is coded on 5 bits
                {
                    // remove the used bit from the current number. (decalage de bit)
                    currentMD5value = currentMD5value >> 1;
                }
                else if (currentMD5position + 1 < _md5.size())
                {
                    // get the next number if the current is over.
                    currentMD5value = -48 + (int)_md5[++currentMD5position].toAscii();
                }
                else
                {
                    // get the next number if the current is over.
                    currentMD5value = -48 + (int)_md5[0].toAscii();
                    currentMD5position = 0;
                }
                //throw Exception("MD5 string is too short for an interval of " + _k + " (k)");

            } while (nbBitInsered < stSize);

        }

        initIndexTable();
    }

    //! Initialize the index table which determine which stego-table is going to be use.
    void StegoTable::initIndexTable()
    {
        int value = 0;
        int currentPosition = 0;

        _indexTable.clear();
        _indexTable.resize((_stTables[0].size() * _k) / 8.0); // floor

        // cut bits by 8
        for (int stNumber = 0; stNumber < _k; stNumber++)
        {
            for (int i = 0; i < _stTables[0].size(); i++)
            {
                value = value << 1;

                value += _stTables[stNumber][i] ? 1 : 0; // convert bool into int

                // When we go a full octet (8 bits read)
                if (stNumber+i!=0 && (stNumber * _stTables[0].size() + i) % 8 == 0 && currentPosition!=_indexTable.size())
                {
                    _indexTable[currentPosition++] = value % _k;
                    value = 0;
                }
            }
        }
    }

    //! Compute a String representing all the stegano tables and the index table
    QString StegoTable::toString()
    {
        QString str = "> Stego tables\n";
        for (int i=0; i < _stTables.size(); i++)
        {
            str += i+": ";
            for (int j = 0; j < _stTables[i].size(); j++)
            {
                str += _stTables[i][j] + QString(",");
            }
            str += "\n";
        }

        str += "> Index Table:\n";
        for (int i = 0; i < _indexTable.size(); i++)
        {
            str += QString::number(_indexTable.at(i));
        }
        return str;
    }
}
