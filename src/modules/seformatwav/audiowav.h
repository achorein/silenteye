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

#ifndef AUDIOWAV_H_
#define AUDIOWAV_H_

#include <audio.h>
#include <encodeddata.h>
#include <logger.h>

using namespace SilentEyeFramework;

namespace SEFormatWAV {

    //! Representation of .wav file (WAVE PCM soundfile format)
    class AudioWav : public Audio
    {
    public:
        enum HeaderPosition{BEGINNING=1, ENDING=2};
        enum DataDistribution{INLINE=1, EQUI=2};

    private:
        quint16 m_nbBitsUsed;
        quint16 m_nbChannelUsed;
        HeaderPosition m_headerPosition;
        DataDistribution m_distribution;

        QPointer<Logger> m_logger;

    public:
        AudioWav(const QString&);
        AudioWav(const Audio&);
        AudioWav(Audio*);
        ~AudioWav();

    private:
        void init();

    public:
        quint16 nbBitsUsed() const;
        void setNbBitsUsed(const quint16);

        quint16 nbChannelUsed() const;
        void setNbChannelUsed(const quint16);

        void setDistribution(DataDistribution);
        void setHeaderPosition(HeaderPosition);

        quint32 capacity() const;

        bool loadData();
        bool saveToDir(QString&);

    protected:
        int computeHeaderSampleSize() const;
        qint32 computeDistributionStep(quint32) const;
        bool writeSampleChannel(QDataStream&, quint32);
    };

}

#endif
