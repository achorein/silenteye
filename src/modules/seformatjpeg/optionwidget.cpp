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

namespace SEFormatJPEG {

    OptionWidget::OptionWidget(QWidget* parent)
        : QWidget(parent)
    {
        this->setObjectName("SEFormatJpegOptionWidget");
        m_logger = new Logger(this);

        setupUi(this);

        connect(pwdCheckBox, SIGNAL(stateChanged(int)), this, SLOT(pwdStateChange(int)));
    }

    OptionWidget::~OptionWidget()
    {

    }

    void OptionWidget::hideQuality(bool hide)
    {
        compressLabel->setVisible(!hide);
        compressSpinBox->setVisible(!hide);
    }

    int OptionWidget::k() const
    {
        return kSlider->value();
    }

    void OptionWidget::setK(const int k)
    {
        if (k >= kSlider->minimum() && k <= kSlider->maximum()) {
            kSlider->setValue(k);
        } else
            m_logger->warning("Invalid value for k: " + k);
    }

    QString OptionWidget::passphrase() const
    {
        return pwdLineEdit->text();
    }

    void OptionWidget::setPassphrase(const QString passphrase)
    {
        pwdLineEdit->setText(passphrase);
    }

    int OptionWidget::quality() const
    {
        return compressSpinBox->value();
    }

    void OptionWidget::setQuality(const int quality) {
        if (quality >= compressSpinBox->minimum() && quality <= compressSpinBox->maximum()) {
            compressSpinBox->setValue(quality);
        } else
            m_logger->warning("Invalid value for k: " + quality);
    }

    ImageJPEG::HeaderPosition OptionWidget::headerPosition() const
    {
        ImageJPEG::HeaderPosition pos = ImageJPEG::SIGNATURE;
        switch(headerComboBox->currentIndex())
        {
            case 0:
                pos = ImageJPEG::TOP;
                break;
            case 1:
                pos = ImageJPEG::BOTTOM;
                break;
            case 2:
                pos = ImageJPEG::SIGNATURE;
                break;
            default:
                m_logger->warning("Unhandle header switch value: " + headerComboBox->currentIndex());
                break;
        }
        return pos;
    }

    void OptionWidget::setHeaderPosition(ImageJPEG::HeaderPosition pos)
    {
        switch(pos)
        {
            case ImageJPEG::TOP:
                headerComboBox->setCurrentIndex(0);
                break;
            case ImageJPEG::BOTTOM:
                headerComboBox->setCurrentIndex(1);
                break;
            case ImageJPEG::SIGNATURE:
                headerComboBox->setCurrentIndex(2);
                break;
        default:
            m_logger->warning("Unhandle header switch value: " + pos);
            break;
        }
    }

    void OptionWidget::setHeaderPosition(const QString pos)
    {
        if (pos.toUpper() == "TOP") {
            setHeaderPosition(ImageJPEG::TOP);
        } else if (pos.toUpper() == "BOTTOM") {
            setHeaderPosition(ImageJPEG::BOTTOM);
        } else if (pos.toUpper() == "SIGNATURE") {
            setHeaderPosition(ImageJPEG::SIGNATURE);
        } else {
            m_logger->warning("Unhandle header switch value: " + pos);
        }
    }

    void OptionWidget::pwdStateChange(int state)
    {
        if (state == Qt::Unchecked)
        {
            pwdLineEdit->setEchoMode(QLineEdit::Password);
        }
        else
        {
            pwdLineEdit->setEchoMode(QLineEdit::Normal);
        }
    }
}
