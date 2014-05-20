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
//  along with SilentEye. If not, see <http://www.gnu.org/licenses/>.

#ifndef AUDIO_H_
#define AUDIO_H_

#include "media.h"

#include <QString>
#include <QtMultimedia/QAudioFormat>

namespace SilentEyeFramework {

    //! Generic representation of audio which can load and hide specific data according to it's format
    class Audio : public Media
    {
    protected:
        //! sound format (byterate, channels, ...)
        QAudioFormat m_format;

        // Chunk description
        //! ChunkSize
        quint32 m_totalSize;
        // Sub chunk format
        //! Subchunk1Size: 16 for PCM. This is the size of the rest of the Subchunk which follows this number
        quint32 m_subFmtSize;
        //! AudioFormat: PCM = 1 (i.e. Linear quantization) Values other than 1 indicate some form of compression.
        quint16 m_audioFormat;
        //! NumChannels: Mono = 1, Stereo = 2, etc.
        quint16 m_numChannels;
        //! SampleRate: 8000, 44100, etc.
        quint32 m_sampleRate;
        //! ByteRate: == SampleRate * NumChannels * BitsPerSample/8
        quint32 m_byteRate;
        //! BlockAlign: == NumChannels * BitsPerSample/8 The number of bytes for one sample including all channels.
        quint16 m_blockAlign;
        //! BitsPerSample: 8 bits = 8, 16 bits = 16, etc.
        quint16 m_bitPerSample;
        // Sub chunk data
        //! Subchunk2Size: == NumSamples * NumChannels * BitsPerSample/8 This is the number of bytes in the data. You can also think of this as the size of the read of the subchunk following this number.
        quint32 m_subDataSize;
        //! byte ordering assumed for WAVE data (little-endian for RIFF, big-endian for RIFX)
        QAudioFormat::Endian m_byteOrder;

        //! length of the recording (in seconds)
        double m_duration;
        //! bit rate (bit/s)
        quint32 m_bitRate;

    private:
        QFile m_file;
        QDataStream m_sampleStream;

    public:
        Audio(const QString& filepath);
        Audio(const Audio&);
        Audio(Audio*);
        ~Audio();

        virtual double duration();
        virtual quint32 bitRate();
        virtual QAudioFormat format() const;

        //! WAV file header length in octets
        static int headerLength() { return 44; }

    protected:
        //! Open current file and set cursor on data
        bool openSamples();
        //! Can we read one more sample ?
        bool hasNextSample();
        //! Read next sample (all channels)
        QList<quint32> readSample();
        //! Skip some sample
        void skipSample(qint32);
        //! Close file
        bool closeSamples();
        //! Number of sample in the file
        qint32 sampleSize() const;

        //! load header information from current wav file
        void readWaveHeader();
        //! write header information on given stream
        void writeWaveHeader(QDataStream&);
    };

}

#endif
