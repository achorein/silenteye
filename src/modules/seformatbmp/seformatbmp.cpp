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

#include "seformatbmp.h"

namespace SEFormatBMP {

    SEFormatBmp::SEFormatBmp()
    {
        this->setObjectName("SEFormatBmp");
        m_logger = new Logger(this);

        m_encodeWidget = NULL;
        m_decodeWidget = NULL;
    }

    SEFormatBmp::~SEFormatBmp()
    {
        if (m_encodeWidget != NULL)
            delete m_encodeWidget;
        if (m_decodeWidget != NULL)
            delete m_decodeWidget;
        delete m_logger;
    }

    QString SEFormatBmp::name() const
    {
        return QString("Silent Eye Image Format BMP");
    }

    QString SEFormatBmp::version() const
    {
        return QString("1.2");
    }

    QString SEFormatBmp::status()
    {
        return "OK|";
    }

    ModuleWidgetInterface* SEFormatBmp::encodeWidget()
    {
        if (m_encodeWidget == NULL)
        {
            m_encodeWidget = new OptionWidget();
            m_encodeWidget->setObjectName("EncodeWidget");
            connect(m_encodeWidget, SIGNAL(optionHasChanged()), this, SLOT(optionHasChanged()));
        }
        return m_encodeWidget;
    }

    ModuleWidgetInterface* SEFormatBmp::decodeWidget()
    {
        if (m_decodeWidget == NULL)
        {
            m_decodeWidget = new OptionWidget();
            m_decodeWidget->setObjectName("DecodeWidget");
        }
        return m_decodeWidget;
    }

    QPointer<Image> SEFormatBmp::encodeImage(QPointer<Image> img, bool dontDuplicate)
    {
        // m_logger->debug("encodeImage");
        return updateImage(img, (OptionWidget*)encodeWidget(), dontDuplicate);
    }

    QPointer<Image> SEFormatBmp::decodeImage(QPointer<Image> img, bool dontDuplicate)
    {
        // m_logger->debug("decodeImage");
        return updateImage(img, (OptionWidget*)decodeWidget(), dontDuplicate);
    }

    QPointer<Image> SEFormatBmp::updateImage(QPointer<Image> img, OptionWidget* w, bool dontDuplicate)
    {
        QPointer<Image> imgBMP;
        if (dontDuplicate)
        {
            imgBMP = new ImageBMP(img); // don't copy image data
        }
        else
        {
            imgBMP = new ImageBMP(*img); // duplicate image into a new one
        }

        ImageBMP* newImage = ((ImageBMP*)imgBMP.data());
        newImage->setNbBits(w->nbBits());
        newImage->setColorUsed(w->isRedUsed(), w->isGreenUsed(), w->isBlueUsed());
        newImage->setDistribution(w->distribution());
        newImage->setHeaderPosition(w->headerPosition());

        return imgBMP;
    }

    QString SEFormatBmp::typeSupported() const
    {
        return QString("BMP");
    }

    void SEFormatBmp::optionHasChanged()
    {
        emit optionChanged();
    }

    }

Q_EXPORT_PLUGIN2(seformatbmp, SEFormatBMP::SEFormatBmp)
