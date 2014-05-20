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
        : ModuleWidgetInterface(parent)
    {
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

    QString OptionWidget::passphrase() const
    {
        return pwdLineEdit->text();
    }

    int OptionWidget::quality() const
    {
        return compressSpinBox->value();
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

    //! Widget is ready for encoding/decoding process
    bool OptionWidget::isReady() const {
        // NOTHING TO CHECK FOR JPEG
        return true;
    }
    //! Export module configuration
    QMap<QString, QString> OptionWidget::exportConfiguration()
    {
        QMap<QString, QString> map;
        map["k"] = QString::number(k());
        map["passphrase"] = passphrase();
        map["quality"] = QString::number(quality());
        map["header"] = QString::number(headerComboBox->currentIndex());
        return map;
    }

    //! Load module configuration
    void OptionWidget::loadConfiguration(QMap<QString, QString> map)
    {
        if (map.contains("k"))
            kSlider->setValue(map["k"].toInt());
        else
            m_logger->warning("loaded configuration didn't have 'k' key");

        if (map.contains("passphrase"))
            pwdLineEdit->setText(map["passphrase"]);
        else
            m_logger->warning("loaded configuration didn't have 'passphrase' key");

        if (map.contains("quality"))
            compressSpinBox->setValue(map["quality"].toInt());
        else
            m_logger->warning("loaded configuration didn't have 'quality' key");

        if (map.contains("header"))
            headerComboBox->setCurrentIndex(map["header"].toInt());
        else
            m_logger->warning("loaded configuration didn't have 'header' key");

        emit optionHasChanged();
    }
}
