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

#include "audiowav.h"

#include <moduleexception.h>

namespace SEFormatWAV {

    AudioWav::AudioWav(const QString& filePath)
        : Audio(filePath)
    {
        init();
    }

    AudioWav::AudioWav(const Audio& audio)
        : Audio(audio)
    {
        init();
    }

    AudioWav::AudioWav(Audio* audio)
        : Audio(audio)
    {
        init();
    }
    
    void AudioWav::init() {
        computeNewFileName("wav");
        setObjectName("AudioWav");
        m_logger = new Logger(this);

        m_nbBitsUsed = 2;
        m_nbChannelUsed = m_numChannels;
        m_headerPosition = ENDING;
        m_distribution = EQUI;
    }
    
    AudioWav::~AudioWav()
    {
        if (!m_logger.isNull())
        {
            delete m_logger;
        }
    }

    quint16 AudioWav::nbBitsUsed() const
    {
        return m_nbBitsUsed;
    }

    void AudioWav::setNbBitsUsed(const quint16 value)
    {
        m_nbBitsUsed = value;
    }

    quint16 AudioWav::nbChannelUsed() const
    {
        return m_nbChannelUsed;
    }

    void AudioWav::setNbChannelUsed(const quint16 value)
    {
        if (value > m_numChannels) {
            m_nbChannelUsed = m_numChannels;
        } else {
            m_nbChannelUsed = value;
        }
    }

    void AudioWav::setDistribution(DataDistribution value)
    {
        m_distribution = value;
    }

    void AudioWav::setHeaderPosition(HeaderPosition value)
    {
        m_headerPosition = value;
    }

    quint32 AudioWav::capacity() const
    {
        /*m_logger->debug("nbSample: " + QString::number(sampleSize())
                        + ";m_nbChannelUsed: " + QString::number(m_nbChannelUsed)
                        + ";m_nbBitsUsed: " + QString::number(m_nbBitsUsed));*/
        return floor(sampleSize() * m_nbChannelUsed * m_nbBitsUsed) / 8.0;
    }
    
    bool AudioWav::loadData()
    {
        m_logger->debug("channels used:" + QString::number(m_nbChannelUsed)
                        + ", bits used: " + QString::number(m_nbBitsUsed));

        EncodedData dataSize(Data::UINT32);
        dataSize.initialize(m_nbBitsUsed);

        if (m_data.isNull())
            m_data = new EncodedData();
        else
            m_data->clear();

        m_data->initialize(m_nbBitsUsed);
        unsigned short int andOperator = EncodedData::andOperator(m_nbBitsUsed);

        if (!openSamples())
        {
            throw ModuleException("Technical error during file reading",
                                  "Cannot read source file! (" + m_filePath + ")");
        }

        if (m_headerPosition == ENDING) {
            m_logger->debug("Skip to header pos: "
                            + QString::number(sampleSize() - computeHeaderSampleSize())
                            + " / " + QString::number(sampleSize())
                            + ". subDataSize: " + QString::number(m_subDataSize)
                            + ", bitPerSample: " + QString::number(m_bitPerSample)
                            + ", numChannels: " + QString::number(m_numChannels));
            skipSample(sampleSize() - computeHeaderSampleSize());
        }

        int nbBitsRead = 0;
        while(hasNextSample() && nbBitsRead<32) {
            QList<quint32> samples = readSample();
            for (int i=0; i<m_nbChannelUsed && nbBitsRead<32; i++)
            {
                int val = samples.at(i)&andOperator;
                dataSize.append(val);
                nbBitsRead += m_nbBitsUsed;
            }
        }

        closeSamples();

        quint32 nbOctets = dataSize.toUInt32();
        m_logger->debug("loaded size: " + QString::number(nbOctets));

        if (nbOctets > capacity()) {
            m_logger->warning("size > capacity");
            return false;
        }

        if (!openSamples())
        {
            throw ModuleException("Technical error during file reading",
                                  "Cannot read source file! (" + m_filePath + ")");
        }

        if (m_headerPosition == BEGINNING) {
            skipSample(computeHeaderSampleSize());
        }

        nbBitsRead = 0;
        int nbBits = nbOctets*8;
        int step = computeDistributionStep(nbOctets);
        while(hasNextSample() && nbBitsRead < nbBits) {
            QList<quint32> samples = readSample();
            for (int i=0; i<m_nbChannelUsed && nbBitsRead < nbBits; i++)
            {
                int val = samples.at(i)&andOperator;
                m_data->append(val);
                nbBitsRead += m_nbBitsUsed;
            }

            skipSample(step-1);
        }

        closeSamples();
        
        m_isLoaded = m_data->size()>0;
        return m_isLoaded;
    }
    
    bool AudioWav::saveToDir(QString& outputDirPath)
    {
        if (!outputDirPath.endsWith('/'))
        {
            outputDirPath += "/";
        }

        m_logger->debug("channels used:" + QString::number(m_nbChannelUsed)
                        + ", bits used: " + QString::number(m_nbBitsUsed));

        if (m_data.isNull())
            throw ModuleException("Technical error during encoding process",
                                  "Cannot insert null data into wave");

        // Open sample using current filepath
        if (!openSamples())
        {
            throw ModuleException("Technical error during file writing",
                                  "Cannot read source file! (" + m_filePath + ")");
        }

        // change filepath for output
        if (QFile::exists(outputDirPath+"/"+m_shortName))
        {
            m_filePath = outputDirPath+"/_"+m_shortName; // for unit test
        }
        else
        {
            m_filePath = outputDirPath+"/"+m_shortName;
        }

        QFile output(m_filePath);

        if (!output.open(QIODevice::WriteOnly))
        {
            throw ModuleException("Technical error during file writing",
                                  "Cannot create destination file! (" + m_filePath + ")");
        }
        QDataStream out(&output);
        writeWaveHeader(out);

        // prepare output for writing
        if (m_byteOrder == QAudioFormat::BigEndian) {
            out.setByteOrder(QDataStream::BigEndian);
        } else {
            out.setByteOrder(QDataStream::LittleEndian);
        }

        EncodedData sizeData(m_data->size());
        m_logger->debug("Setted size: " + QString::number(sizeData.toUInt32())
                        + "/" + QString::number(capacity()));

        sizeData.initialize(m_nbBitsUsed);

        int swap = 255 - EncodedData::andOperator(m_nbBitsUsed);
        qint32 sampleIndex = 0;

        if (m_headerPosition == BEGINNING) {
            while(hasNextSample() && sizeData.hasNext()) {
                QList<quint32> samples = readSample();
                sampleIndex ++;
                int i;
                for (i=0; i<m_nbChannelUsed && sizeData.hasNext(); i++)
                {
                    int val = sizeData.read();
                    quint32 sample = samples.at(i);
                    sample = (sample&swap)+val;

                    writeSampleChannel(out, sample);
                }

                for (;i<samples.size();i++)
                {
                    writeSampleChannel(out, samples.at(i));
                }
            }
        }

        int headNbSample = computeHeaderSampleSize();
        qint32 step = computeDistributionStep(m_data->size());

        m_data->initialize(m_nbBitsUsed);
        while(hasNextSample() && m_data->hasNext()) {
            QList<quint32> samples = readSample();
            sampleIndex ++;
            int i;
            for (i=0; i<m_nbChannelUsed && m_data->hasNext(); i++)
            {
                int val = m_data->read();
                quint32 sample = samples.at(i);
                sample = (sample&swap)+val;

                writeSampleChannel(out, sample);
            }

            for (;i<samples.size();i++)
            {
                writeSampleChannel(out, samples.at(i));
            }

            if (sampleIndex + step < sampleSize()-headNbSample) {
                for(int j=0; j < step-1 && hasNextSample(); j++) {
                    samples = readSample();
                    sampleIndex ++;
                    for (int i=0;i<samples.size();i++)
                    {
                        writeSampleChannel(out, samples.at(i));
                    }
                }
            }
        }
        m_logger->debug("Last data pos: " + QString::number(sampleIndex));

        while(hasNextSample())
        {
            QList<quint32> samples = readSample();
            sampleIndex ++;
            if (m_headerPosition == ENDING && sampleIndex > sampleSize()-headNbSample)
            {
                if (sampleIndex == sampleSize()-headNbSample+1) {
                    m_logger->debug("Header start pos: " + QString::number(sampleIndex));
                }
                int i;
                for (i=0; i<m_nbChannelUsed && sizeData.hasNext(); i++)
                {
                    int val = sizeData.read();
                    quint32 sample = samples.at(i);
                    sample = (sample&swap)+val;

                    writeSampleChannel(out, sample);
                }

                for (;i<samples.size();i++)
                {
                    writeSampleChannel(out, samples.at(i));
                }
            } else {
                for (int i=0;i<samples.size();i++)
                {
                    writeSampleChannel(out, samples.at(i));
                }
            }
        }

        m_logger->debug("Last sample pos: " + QString::number(sampleIndex)
                        + " / " + QString::number(sampleSize()) + "."
                        + " headNbSample: " + QString::number(headNbSample)
                        + ", dataSize: " + QString::number(m_subDataSize)
                        + ", bitPerSample: " + QString::number(m_bitPerSample)
                        + ", numChannels: " + QString::number(m_numChannels));
        closeSamples();

        output.close();
        return true;
    }

    int AudioWav::computeHeaderSampleSize() const
    {
        return ceil(32.0 /  (m_nbChannelUsed * m_nbBitsUsed));
    }

    qint32 AudioWav::computeDistributionStep(quint32 size) const {
        qint32 step = 1;
        if(m_distribution == EQUI)
        {
            qint32 sampleDataNeed = ceil((size * 8.0)/(m_nbChannelUsed*m_nbBitsUsed));
            step = floor(((double)sampleSize()) / sampleDataNeed);
            if (step <= 0) {
                m_logger->debug("computed step was 0 => set 1");
                step = 1;
            }
            m_logger->debug("computed step: " + QString::number(step)
                            + ", size: " + QString::number(size));
        }
        return step;
    }

    bool AudioWav::writeSampleChannel(QDataStream& out, quint32 sample) {
        if (m_bitPerSample == 8) {
            out << quint8(sample);
        } else if (m_bitPerSample == 16) {
            out << quint16(sample);
        } else if (m_bitPerSample == 32) {
            out << sample;
        } else {
            m_logger->warning("bitpersample: " + QString::number(m_bitPerSample));
            return false;
        }
        return true;
    }
}
