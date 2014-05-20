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

#include "optionwidget.h"

namespace SEFormatWAV {

    OptionWidget::OptionWidget(QWidget* parent)
        : ModuleWidgetInterface(parent)
    {
        this->setObjectName("SEFormatWaveOptionWidget");
        m_logger = new Logger(this);

        m_dontUpdate = false;

        setupUi(this);
        advancedWidget->setVisible(false);

        settingsChanged();

        connect(advancedButton, SIGNAL(toggled(bool)), this, SLOT(advButtonPressed(bool)));
        connect(qualityComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(qualityChanged(int)));
        connect(channelsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsChanged(int)));
        connect(nbBitsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(settingsChanged(int)));
    }

    OptionWidget::~OptionWidget()
    {
        delete m_logger;
    }

    int OptionWidget::nbBits() const
    {
        return nbBitsSpinBox->value();
    }

    int OptionWidget::channels() const
    {
        return channelsComboBox->currentIndex() + 1;
    }

    AudioWav::DataDistribution OptionWidget::distribution() const
    {
        AudioWav::DataDistribution dist = AudioWav::EQUI;
        switch(distributionComboBox->currentIndex())
        {
            case 0:
                dist = AudioWav::EQUI;
                break;
            case 1:
                dist = AudioWav::INLINE;
                break;
            default:
                m_logger->warning("Unhandle distribution switch value: " + QString::number(distributionComboBox->currentIndex()));
                break;
        }
        return dist;
    }

    AudioWav::HeaderPosition OptionWidget::headerPosition() const
    {
        AudioWav::HeaderPosition pos = AudioWav::ENDING;
        switch(headerComboBox->currentIndex())
        {
            case 0:
                pos = AudioWav::BEGINNING;
                break;
            case 1:
                pos = AudioWav::ENDING;
                break;
            default:
                m_logger->warning("Unhandle header switch value: " + headerComboBox->currentIndex());
                break;
        }
        return pos;
    }

    void OptionWidget::advButtonPressed(bool value)
    {
        advancedWidget->setVisible(value);
    }

    void OptionWidget::qualityChanged(int value)
    {
        if (!m_dontUpdate)
        {
            m_dontUpdate = true;
            switch(value)
            {
                case 0: // low
                    channelsComboBox->setCurrentIndex(1);
                    nbBitsSpinBox->setValue(5);
                    headerComboBox->setCurrentIndex(0);
                    break;
                case 1: // normal
                    channelsComboBox->setCurrentIndex(1);
                    nbBitsSpinBox->setValue(3);
                    headerComboBox->setCurrentIndex(1);
                    break;
                case 2: // high
                    channelsComboBox->setCurrentIndex(0);
                    nbBitsSpinBox->setValue(1);
                    headerComboBox->setCurrentIndex(1);
                    break;
                default:
                    m_logger->warning("Unhandle quality mode switch value: " + QString::number(value));
                    break;
            }
            m_dontUpdate = false;
            settingsChanged();
        }
    }

    void OptionWidget::settingsChanged(int value)
    {
        Q_UNUSED(value);

        if (!m_dontUpdate)
        {
            m_dontUpdate = true;
            int nbChannel = channelsComboBox->currentIndex()+1;

            m_logger->debug("nb channel used: " + QString::number(nbChannel) + ", nb bits: " + QString::number(nbBits()));

            double quality = (1 - ((pow(2, nbBits()) * nbChannel) / 256.0)) * 100.0;
            pourcentLabel->setText(QString::number(quality) + "%");
            m_logger->debug("computed quality: " + QString::number(quality));

            if (quality > 95)
                qualityComboBox->setCurrentIndex(2); // high
            else if (quality > 85)
                qualityComboBox->setCurrentIndex(1); // normal
            else
                qualityComboBox->setCurrentIndex(0); // low

            emit optionHasChanged();
            m_dontUpdate = false;
        }
    }

    //! Widget is ready for encoding/decoding process
    bool OptionWidget::isReady() const {
        // NOTHING TO CHECK FOR BMP
        return true;
    }

    //! Export module configuration
    QMap<QString, QString> OptionWidget::exportConfiguration()
    {
        QMap<QString, QString> map;
        map["nb_bits"] = QString::number(nbBits());
        map["channels"] = QString::number(channels());
        map["header"] = QString::number(headerComboBox->currentIndex());
        map["distribution"] = QString::number(distributionComboBox->currentIndex());
        return map;
    }

    //! Load module configuration
    void OptionWidget::loadConfiguration(QMap<QString, QString> map)
    {
        m_dontUpdate = true;

        if (map.contains("nb_bits"))
            nbBitsSpinBox->setValue(map["nb_bits"].toInt());
        else
            m_logger->warning("loaded configuration didn't have 'nb_bits' key");

        if (map.contains("channels"))
            channelsComboBox->setCurrentIndex(map["channels"].toInt());
        else
            m_logger->warning("loaded configuration didn't have 'channels' key");

        if (map.contains("header"))
            headerComboBox->setCurrentIndex(map["header"].toInt());
        else
            m_logger->warning("loaded configuration didn't have 'header' key");

        if (map.contains("distribution"))
            distributionComboBox->setCurrentIndex(map["distribution"].toInt());
        else
            m_logger->warning("loaded configuration didn't have 'distribution' key");

        emit optionHasChanged();
        m_dontUpdate = false;
    }

}
