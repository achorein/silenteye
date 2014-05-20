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
        #ifdef CONFPATH_USERHOME
            QString config_path = QDir::homePath()+"/.silenteye/";
        #elif defined(CONFPATH_VALUE)
            QString config_path = CONFPATH_VALUE + QString("/");
        #else
            QString config_path = qApp->applicationDirPath()+"/";
        #endif
        m_config = new Config(config_path, "seformatjpeg");
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
        return "OK|Configuration file " + m_config->fileAbsoluteName()
                + " " + (m_config->isLoaded() ? "loaded." : "not loaded.");
    }

    QWidget* SEFormatJpeg::encodeWidget()
    {
        if (m_encodeWidget == NULL)
        {
            m_encodeWidget = new OptionWidget();
            m_encodeWidget->setObjectName("EncodeWidget");
            if (!m_config.isNull() && m_config->isLoaded()) {
                m_logger->info("Load encode default options from configuration file: " + m_config->fileAbsoluteName());
                m_encodeWidget->setK(m_config->getInt("k"));
                m_encodeWidget->setPassphrase(m_config->get("passphrase"));
                m_encodeWidget->setQuality(m_config->getInt("quality"));
                m_encodeWidget->setHeaderPosition(m_config->get("headerPosition"));
            }
            connect(m_encodeWidget, SIGNAL(optionHasChanged()), this, SLOT(optionHasChanged()));
        }
        return m_encodeWidget;
    }

    QWidget* SEFormatJpeg::decodeWidget()
    {
        if (m_decodeWidget == NULL)
        {
            m_decodeWidget = new OptionWidget();
            m_decodeWidget->hideQuality();
            m_decodeWidget->setObjectName("DecodeWidget");
            if (!m_config.isNull() && m_config->isLoaded()) {
                m_logger->info("Load decode default options from configuration file: " + m_config->fileAbsoluteName());
                m_decodeWidget->setK(m_config->getInt("k"));
                m_decodeWidget->setPassphrase(m_config->get("passphrase"));
                m_decodeWidget->setQuality(m_config->getInt("quality"));
                m_decodeWidget->setHeaderPosition(m_config->get("headerPosition"));
            }
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

    bool SEFormatJpeg::isEncodeWidgetReady() const
    {
        return true;
    }

    bool SEFormatJpeg::isDecodeWidgetReady() const
    {
        return true;
    }

    void SEFormatJpeg::optionHasChanged()
    {
        emit optionChanged();
    }

}

Q_EXPORT_PLUGIN2(seformatjpeg, SEFormatJPEG::SEFormatJpeg)
