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

#ifndef OPTIONWIDGET_H_
#define OPTIONWIDGET_H_

#include <QtGui>
#include <logger.h>
#include "ui_optionwidget.h"
#include "audiowav.h"

using namespace SilentEyeFramework;

namespace SEFormatWAV {

    class OptionWidget : public QWidget, public Ui::OptionWidgetSilentEyeWav
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

        int channels() const;
        void setNbChannelUsed(const int);

        AudioWav::DataDistribution distribution() const;
        void setDistribution(const QString);
        void setDistribution(const AudioWav::DataDistribution);

        AudioWav::HeaderPosition headerPosition() const;
        void setHeaderPosition(AudioWav::HeaderPosition);
        void setHeaderPosition(const QString);

    protected slots:
        void advButtonPressed(bool);
        void qualityChanged(int=-1);
        void settingsChanged(int=-1);

    signals:
        void optionHasChanged();
    };

}

#endif
