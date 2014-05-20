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
#include <math.h>

namespace SEFormatBMP {

    OptionWidget::OptionWidget(QWidget* parent)
        : QWidget(parent)
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

    void OptionWidget::setNbBits(const int nbBits)
    {
        if (nbBits >= nbBitsSpinBox->minimum() && nbBits <= nbBitsSpinBox->maximum()) {
            nbBitsSpinBox->setValue(nbBits);
            settingsChanged();
        } else
            m_logger->warning("Invalid value for nbBits: " + nbBits);
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

    void OptionWidget::setColorUsed(const bool red, const bool green, const bool blue) {
        redCheckBox->setChecked(red);
        greenCheckBox->setChecked(green);
        blueCheckBox->setChecked(blue);
        settingsChanged();
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

    void OptionWidget::setDistribution(const ImageBMP::DataDistribution dist)
    {
        switch(dist)
        {
            case ImageBMP::EQUI:
                distributionComboBox->setCurrentIndex(0);
                break;
            case ImageBMP::INLINE:
                distributionComboBox->setCurrentIndex(1);
                break;
        default:
            m_logger->warning("Unhandle distribution switch value: " + dist);
            break;
        }
        settingsChanged();
    }

    void OptionWidget::setDistribution(const QString dist)
    {
        if (dist.toUpper() == "EQUI") {
            setDistribution(ImageBMP::EQUI);
        } else if (dist.toUpper() == "INLINE") {
            setDistribution(ImageBMP::INLINE);
        } else {
            m_logger->warning("Unhandle distribution switch value: " + dist);
        }
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

    void OptionWidget::setHeaderPosition(ImageBMP::HeaderPosition pos)
    {
        switch(pos)
        {
            case ImageBMP::TOP:
                headerComboBox->setCurrentIndex(0);
                break;
            case ImageBMP::BOTTOM:
                headerComboBox->setCurrentIndex(1);
                break;
            case ImageBMP::SIGNATURE:
                headerComboBox->setCurrentIndex(2);
                break;
        default:
            m_logger->warning("Unhandle header switch value: " + pos);
            break;
        }
        settingsChanged();
    }

    void OptionWidget::setHeaderPosition(const QString pos)
    {
        if (pos.toUpper() == "TOP") {
            setHeaderPosition(ImageBMP::TOP);
        } else if (pos.toUpper() == "BOTTOM") {
            setHeaderPosition(ImageBMP::BOTTOM);
        } else if (pos.toUpper() == "SIGNATURE") {
            setHeaderPosition(ImageBMP::SIGNATURE);
        } else {
            m_logger->warning("Unhandle header switch value: " + pos);
        }
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

}
