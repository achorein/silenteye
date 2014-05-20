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

#ifndef SEFORMATJPEG_H_
#define SEFORMATJPEG_H_

#include <QtCore>
#include <imagemoduleinterface.h>
#include <logger.h>
#include <config.h>
#include <image.h>

#include "optionwidget.h"

using namespace SilentEyeFramework;

//! SilentEye official module for support of JPEG image
namespace SEFormatJPEG {


    //! Official module for support of JPEG image (using LSB)
    class SEFormatJpeg : public ImageModuleInterface
    {
        Q_OBJECT;
        Q_INTERFACES(SilentEyeFramework::ImageModuleInterface);

    private:
        QPointer<Logger> m_logger;
        OptionWidget* m_encodeWidget;
        OptionWidget* m_decodeWidget;
        QPointer<Config> m_config;

    public:
        SEFormatJpeg();
        ~SEFormatJpeg();

        QWidget* encodeWidget();
        QWidget* decodeWidget();
        bool isEncodeWidgetReady() const;
        bool isDecodeWidgetReady() const;

        QPointer<Image> encodeImage(QPointer<Image>,bool=false);
        QPointer<Image> decodeImage(QPointer<Image>,bool=false);

        QString typeSupported() const;

        QString name() const;
        QString version() const;
        QString status();

    private:
        QPointer<Image> updateImage(QPointer<Image>, OptionWidget*, bool);

    private slots:
        void optionHasChanged();

    signals:
        void optionChanged();
    };
}

#endif
