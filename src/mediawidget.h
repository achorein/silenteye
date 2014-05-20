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

#ifndef MEDIAWIDGET_H_
#define MEDIAWIDGET_H_

#include <QWidget>
#include <QPixmap>
#include <QMenu>
#include <QContextMenuEvent>
#include "ui_mediawidget.h"

//! SilentEye application GUI
namespace SilentEye {

    //! Manage media view into main window
    class MediaWidget : public QWidget, public Ui::MediaWidget
    {
        Q_OBJECT;

    private:
        QMenu* m_menu;
        QSize m_size;
        bool m_hasImg;
        QString m_filepath;

    public:
        MediaWidget(QWidget* parent=0, QString filepath="");
        ~MediaWidget();

        void setPixmap(const QPixmap, bool=true);
        void setText(const QString);
        void setFilepath(const QString);
        void setContextMenu(QMenu*);

        void showPlayer(bool);
        void contextMenuEvent(QContextMenuEvent*);
        QString filepath() const;

        void updateImageSize();
        void resizeEvent(QResizeEvent*);

    signals:
        void play();
        void stop();

    private slots:
        void playPressed();
        void stopPressed();

    };

}

#endif

