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

#include "seformatjpeg.h"
#include "imagejpeg.h"

namespace SEFormatJPEG {

    SEFormatJpeg::SEFormatJpeg()
    {
        this->setObjectName("SEFormatJpeg");
        m_logger = new Logger(this);

        m_encodeWidget = NULL;
        m_decodeWidget = NULL;
    }

    SEFormatJpeg::~SEFormatJpeg()
    {
        if (m_encodeWidget != NULL)
            delete m_encodeWidget;
        if (m_decodeWidget != NULL)
            delete m_decodeWidget;
        delete m_logger;
    }

    QString SEFormatJpeg::name() const
    {
        return QString("Silent Eye Image Format JPEG");
    }

    QString SEFormatJpeg::version() const
    {
        return QString("1.1");
    }

    QString SEFormatJpeg::status()
    {
        return "OK|";
    }

    ModuleWidgetInterface* SEFormatJpeg::encodeWidget()
    {
        if (m_encodeWidget == NULL)
        {
            m_encodeWidget = new OptionWidget();
            m_encodeWidget->setObjectName("EncodeWidget");
            connect(m_encodeWidget, SIGNAL(optionHasChanged()), this, SLOT(optionHasChanged()));
        }
        return m_encodeWidget;
    }

    ModuleWidgetInterface* SEFormatJpeg::decodeWidget()
    {
        if (m_decodeWidget == NULL)
        {
            m_decodeWidget = new OptionWidget();
            m_decodeWidget->hideQuality();
            m_decodeWidget->setObjectName("DecodeWidget");
        }
        return m_decodeWidget;
    }

    QPointer<Image> SEFormatJpeg::encodeImage(QPointer<Image> img, bool dontDuplicate)
    {
        return updateImage(img, (OptionWidget*) encodeWidget(), dontDuplicate);
    }

    QPointer<Image> SEFormatJpeg::decodeImage(QPointer<Image> img, bool dontDuplicate)
    {
        return updateImage(img, (OptionWidget*) decodeWidget(), dontDuplicate);
    }

    QPointer<Image> SEFormatJpeg::updateImage(QPointer<Image> img, OptionWidget* w, bool dontDuplicate)
    {
        QPointer<Image> imgJpeg;
        if (dontDuplicate)
        {
            imgJpeg = new ImageJPEG(img); // don't copy image data
        }
        else
        {
            imgJpeg = new ImageJPEG(*img); // duplicate image into a new one
        }

        ImageJPEG* newImage = ((ImageJPEG*)imgJpeg.data());
        newImage->setK(w->k());
        newImage->setPassphrase(w->passphrase());
        newImage->setQuality(w->quality());
        newImage->setHeaderPosition(w->headerPosition());

        return newImage;
    }

    QString SEFormatJpeg::typeSupported() const
    {
        return QString("JPEG");
    }

    void SEFormatJpeg::optionHasChanged()
    {
        emit optionChanged();
    }

}

Q_EXPORT_PLUGIN2(seformatjpeg, SEFormatJPEG::SEFormatJpeg)
