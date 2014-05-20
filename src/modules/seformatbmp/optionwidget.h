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

#ifndef OPTIONWIDGETBMP_H_
#define OPTIONWIDGETBMP_H_

#include <QtGui>
#include "ui_optionwidget.h"

#include <logger.h>
#include <QPointer>

#include "imagebmp.h"

using namespace SilentEyeFramework;

//! SilentEye official module for support of BMP image
namespace SEFormatBMP {

    class OptionWidget : public QWidget, public Ui::OptionWidgetSilentEyeBmp
    {
        Q_OBJECT;

    private:
        QPointer<Logger> m_logger;
        bool m_dontUpdate;

    public:
        OptionWidget(QWidget* parent=0);
        ~OptionWidget();

        int nbBits() const;
        void setNbBits(const int);

        bool isRedUsed() const;
        bool isGreenUsed() const;
        bool isBlueUsed() const;
        void setColorUsed(const bool, const bool, const bool);

        ImageBMP::DataDistribution distribution() const;
        void setDistribution(const QString);
        void setDistribution(const ImageBMP::DataDistribution);
        ImageBMP::HeaderPosition headerPosition() const;
        void setHeaderPosition(ImageBMP::HeaderPosition);
        void setHeaderPosition(const QString);
        QString distributionKey() const;

    protected slots:
        void advButtonPressed(bool);
        void qualityChanged(int=-1);
        void settingsChanged(int=-1);

    signals:
        void optionHasChanged();
    };
}

#endif
