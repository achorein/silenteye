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

#include "audio.h"

#include "moduleexception.h"

namespace SilentEyeFramework {

    /*Audio::Audio(const QAudioFormat &format, qint64 dataLength)
        : m_format(format),  m_dataLength(dataLength)
    {
        setObjectName("Audio");
        m_type = AUDIO;
    }*/

    Audio::Audio(const QString& filePath)
        : Media(filePath)
    {
        setObjectName("Audio");
        m_type = AUDIO;

        readWaveHeader();
    }

    Audio::Audio(const Audio& aud)
        : Media(aud)
    {
        setObjectName("Audio");
        m_type = AUDIO;

        readWaveHeader();
    }

    Audio::Audio(Audio* aud)
        : Media(aud)
    {
        setObjectName("Audio");
        m_type = AUDIO;

        readWaveHeader();
    }

    Audio::~Audio()
    {
        // NOTHING TO DO
    }

    void Audio::readWaveHeader() {
        QFile file(m_filePath);
        if (!file.open(QIODevice::ReadOnly))
        {
            throw ModuleException("Cannot read selected file: " + m_filePath, file.errorString());
        }

        QDataStream stream(&file);
        stream.setByteOrder(QDataStream::BigEndian);

        // CHUNK DESCRIPTION
        quint32 len = 4;
        char* buffer = new char[len+1];
        stream.readRawData(buffer, len); // ChunkID
        buffer[len] = '\0';
        QString identifier(buffer);
        delete buffer;

        if (identifier == "RIFF")
        {
            m_byteOrder = QAudioFormat::BigEndian;
        }
        else if (identifier == "RIFX ")
        {
            m_byteOrder = QAudioFormat::LittleEndian;
        }
        else
        {
            throw ModuleException("Selected file is not in a valid/supported WAVE format.",
                                  "Identifier must be TIFF or RIFX (found: '" + identifier + "'')");
        }
        m_format.setByteOrder(m_byteOrder);

        stream.setByteOrder(QDataStream::LittleEndian);
        stream >> m_totalSize; // ChunkSize

        buffer = new char[len+1];
        stream.readRawData(buffer, len); // Format
        buffer[len] = '\0';
        if ("WAVE" != QString(buffer))
        {
            delete buffer;
            throw ModuleException("Selected file is not in a valid WAVE format.",
                                  "Format must be \"WAVE\"!");
        }
        delete buffer;

        // SUB CHUNK FMT
        buffer = new char[len+1];
        stream.readRawData(buffer, len); // Subchunk1ID
        buffer[len] = '\0';
        if("fmt " != QString(buffer))
        {
            throw ModuleException("Selected file is not in a valid WAVE format",
                                  "Subchunk1ID must be \"fmt \"!");
            delete buffer;
        }
        delete buffer;

        stream >> m_subFmtSize;
        stream >> m_audioFormat;
        if (m_audioFormat != 1 || m_subFmtSize != 16)
        {
            throw ModuleException("Selected file is not in a valid/supported WAVE format.",
                                  "Audio format must be 1 for PCM!");
        }
        m_format.setCodec("audio/pcm");
        stream >> m_numChannels;
        m_format.setChannels(m_numChannels);
        stream >> m_sampleRate;
        m_format.setFrequency(m_sampleRate);
        stream >> m_byteRate;
        stream >> m_blockAlign;
        stream >> m_bitPerSample;
        m_format.setSampleSize(m_bitPerSample);
        if (m_bitPerSample == 8) {
            m_format.setSampleType(QAudioFormat::UnSignedInt);
        } else if (m_bitPerSample == 16) {
            m_format.setSampleType(QAudioFormat::SignedInt);
        } else {
            throw ModuleException("Selected file is not in a valid/supported WAVE format.",
                                  "BitsPerSample must be 8 or 16!");
        }


        // SUB CHUNK DATA
        len = 4;
        buffer = new char[len+1];
        stream.readRawData(buffer, len);
        buffer[len] = '\0';
        if("data" != QString(buffer))
        {
            throw ModuleException("Selected file is not in a valid/supported WAVE format.",
                                  "Subchunk2ID mus be \"data\"!");
            delete buffer;
        }
        delete buffer;

        stream >> m_subDataSize;

        file.close();

        m_duration = (m_subDataSize / (m_bitPerSample/8.0)) / (m_sampleRate * m_numChannels);
        m_bitRate = floor((m_subDataSize * 8.0) / m_duration);
    }

    QAudioFormat Audio::format() const
    {
        return m_format;
    }

    double Audio::duration()
    {
        return m_duration;
    }

    quint32 Audio::bitRate()
    {
        return m_bitRate;
    }

    qint32 Audio::sampleSize() const {
        return floor((m_subDataSize * 8.0) / (m_bitPerSample * m_numChannels));
    }

    bool Audio::openSamples()
    {
        if (m_file.isOpen()) {
            m_file.close();
        }
        m_file.setFileName(m_filePath);
        if(!m_file.open(QIODevice::ReadOnly)) {
            return false;
        }
        m_sampleStream.setDevice(&m_file);
        m_sampleStream.skipRawData(headerLength());

        if (m_byteOrder == QAudioFormat::BigEndian) {
            m_sampleStream.setByteOrder(QDataStream::BigEndian);
        } else {
            m_sampleStream.setByteOrder(QDataStream::LittleEndian);
        }

        return true;
    }

    bool Audio::hasNextSample()
    {
        if (!m_file.isOpen()) {
            return false;
        }
        return !m_sampleStream.atEnd();
    }

    QList<quint32> Audio::readSample()
    {
        QList<quint32> sample;
        for (int i=0; i<m_numChannels; i++) {
            if (m_bitPerSample == 8) {
                quint8 val;
                m_sampleStream >> val;
                sample << val;
            } else if (m_bitPerSample == 16) {
                quint16 val;
                m_sampleStream >> val;
                sample << val;
            }
        }
        return sample;
    }

    void Audio::skipSample(qint32 length) {
        int skip = length * m_numChannels * (m_bitPerSample/8.0);;
        m_sampleStream.skipRawData(skip);
    }

    bool Audio::closeSamples()
    {
        if (m_file.isOpen()) {
            m_file.close();
            m_sampleStream.resetStatus();
            return true;
        } else {
            return false;
        }
    }

    void Audio::writeWaveHeader(QDataStream& stream)
    {
        stream.setByteOrder(QDataStream::BigEndian);

        // CHUNK DESCRIPTION
        if (m_byteOrder == QAudioFormat::LittleEndian) {
            stream.writeRawData("RIFX", 4);
        } else {
            stream.writeRawData("RIFF", 4);
        }

        stream.setByteOrder(QDataStream::LittleEndian);
        stream << m_totalSize; // ChunkSize

        stream.writeRawData("WAVE", 4); // Format

        // SUB CHUNK FMT
        stream.writeRawData("fmt ", 4); // Subchunk1ID

        stream << m_subFmtSize;
        stream << m_audioFormat;
        stream << m_numChannels;
        stream << m_sampleRate;
        stream << m_byteRate;
        stream << m_blockAlign;
        stream << m_bitPerSample;

        // SUB CHUNK DATA
        stream.writeRawData("data", 4);
        stream << m_subDataSize;
    }
}
