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

#include <QWidget>
#include "optionwidget.h"
#include <math.h>

namespace SEFormatBMP {

    OptionWidget::OptionWidget(QWidget* parent)
        : ModuleWidgetInterface(parent)
    {
        this->setObjectName("SEFormatBmpOptionWidget");
        m_logger = new Logger(this);
        m_dontUpdate = false;

        setupUi(this);
        advancedWidget->setVisible(false);

        settingsChanged();

        connect(advancedButton, SIGNAL(toggled(bool)), this, SLOT(advButtonPressed(bool)));
        connect(qualityComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(qualityChanged(int)));
        connect(redCheckBox, SIGNAL(stateChanged(int)), this, SLOT(settingsChanged(int)));
        connect(greenCheckBox, SIGNAL(stateChanged(int)), this, SLOT(settingsChanged(int)));
        connect(blueCheckBox, SIGNAL(stateChanged(int)), this, SLOT(settingsChanged(int)));
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

    bool OptionWidget::isRedUsed() const
    {
        return redCheckBox->checkState() != Qt::Unchecked;
    }

    bool OptionWidget::isGreenUsed() const
    {
        return greenCheckBox->checkState() != Qt::Unchecked;
    }

    bool OptionWidget::isBlueUsed() const
    {
        return blueCheckBox->checkState() != Qt::Unchecked;
    }

    ImageBMP::DataDistribution OptionWidget::distribution() const
    {
        ImageBMP::DataDistribution dist = ImageBMP::EQUI;
        switch(distributionComboBox->currentIndex())
        {
            case 0:
                dist = ImageBMP::EQUI;
                break;
            case 1:
                dist = ImageBMP::INLINE;
                break;
            default:
                m_logger->warning("Unhandle distribution switch value: " + QString::number(distributionComboBox->currentIndex()));
                break;
        }
        return dist;
    }

    ImageBMP::HeaderPosition OptionWidget::headerPosition() const
    {
        ImageBMP::HeaderPosition pos = ImageBMP::SIGNATURE;
        switch(headerComboBox->currentIndex())
        {
            case 0:
                pos = ImageBMP::TOP;
                break;
            case 1:
                pos = ImageBMP::BOTTOM;
                break;
            case 2:
                pos = ImageBMP::SIGNATURE;
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
                case 0: // very low
                    redCheckBox->setChecked(true);
                    greenCheckBox->setChecked(true);
                    blueCheckBox->setChecked(true);
                    nbBitsSpinBox->setValue(6);
                    headerComboBox->setCurrentIndex(0);
                    break;
                case 1: // low
                    redCheckBox->setChecked(true);
                    greenCheckBox->setChecked(true);
                    blueCheckBox->setChecked(true);
                    nbBitsSpinBox->setValue(5);
                    headerComboBox->setCurrentIndex(0);
                    break;
                case 2: // normal
                    redCheckBox->setChecked(true);
                    greenCheckBox->setChecked(true);
                    blueCheckBox->setChecked(true);
                    nbBitsSpinBox->setValue(3);
                    headerComboBox->setCurrentIndex(2);
                    break;
                case 3: // high
                    redCheckBox->setChecked(true);
                    greenCheckBox->setChecked(true);
                    blueCheckBox->setChecked(true);
                    nbBitsSpinBox->setValue(2);
                    headerComboBox->setCurrentIndex(2);
                    break;
                case 4: // best
                    redCheckBox->setChecked(false);
                    greenCheckBox->setChecked(false);
                    blueCheckBox->setChecked(true);
                    nbBitsSpinBox->setValue(1);
                    headerComboBox->setCurrentIndex(2);
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
            int nbPixel = 0;
            if (isRedUsed()) {
                nbPixel++;
            }
            if (isGreenUsed()) {
                nbPixel++;
            }
            if (isBlueUsed()) {
                nbPixel++;
            }
            m_logger->debug("nb pixel used: " + QString::number(nbPixel) + ", nb bits: " + QString::number(nbBits()));

            double quality = (1 - ((pow(2, nbBits()) * nbPixel) / 768.0)) * 100.0;
            pourcentLabel->setText(QString::number(quality) + "%");
            m_logger->debug("computed quality: " + QString::number(quality));

            if (quality > 99)
                qualityComboBox->setCurrentIndex(4); // best
            else if (quality > 98)
                qualityComboBox->setCurrentIndex(3); // high
            else if (quality > 95)
                qualityComboBox->setCurrentIndex(2); // normal
            else if (quality > 85)
                qualityComboBox->setCurrentIndex(1); // low
            else
                qualityComboBox->setCurrentIndex(0); // very low

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
        map["header"] = QString::number(headerComboBox->currentIndex());
        map["distribution"] = QString::number(distributionComboBox->currentIndex());
        map["red"] = (redCheckBox->isChecked())?"1":"0";
        map["green"] = (greenCheckBox->isChecked())?"1":"0";
        map["blue"] = (blueCheckBox->isChecked())?"1":"0";
        map["nb_bits"] = QString::number(nbBitsSpinBox->value());
        return map;
    }

    //! Load module configuration
    void OptionWidget::loadConfiguration(QMap<QString, QString> map)
    {
        m_dontUpdate = true;
        if (map.contains("header"))
            headerComboBox->setCurrentIndex(map["header"].toInt());
        else
            m_logger->warning("loaded configuration didn't have 'header' key");

        if (map.contains("distribution"))
            distributionComboBox->setCurrentIndex(map["distribution"].toInt());
        else
            m_logger->warning("loaded configuration didn't have 'distribution' key");

        if (map.contains("red"))
            redCheckBox->setChecked(map["red"].toInt() == 1);
        else
            m_logger->warning("loaded configuration didn't have 'red' key");

        if (map.contains("green"))
            greenCheckBox->setChecked(map["green"].toInt() == 1);
        else
            m_logger->warning("loaded configuration didn't have 'green' key");

        if (map.contains("blue"))
            blueCheckBox->setChecked(map["blue"].toInt() == 1);
        else
            m_logger->warning("loaded configuration didn't have 'blue' key");

        if (map.contains("nb_bits"))
            nbBitsSpinBox->setValue(map["nb_bits"].toInt());
        else
            m_logger->warning("loaded configuration didn't have 'nb_bits' key");

        emit optionHasChanged();
        m_dontUpdate = false;
    }

}
