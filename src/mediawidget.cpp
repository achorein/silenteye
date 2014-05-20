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

#include "mediawidget.h"

namespace SilentEye {

    MediaWidget::MediaWidget(QWidget* parent, QString filepath)
        : QWidget(parent)
    {
        setupUi(this);
        m_menu = NULL;
        m_hasImg = false;
        playerWidget->setVisible(false);
        setFilepath(filepath);

        connect(playButton, SIGNAL(pressed()), this, SLOT(playPressed()));
        connect(stopButton, SIGNAL(pressed()), this, SLOT(stopPressed()));
    }

    MediaWidget::~MediaWidget() {
        // NOTHING TO DO
    }

    void MediaWidget::setPixmap(const QPixmap pix, bool autosize)
    {
        imageLabel->setPixmap(pix);
        if (autosize)
        {
            m_hasImg = true;
            m_size = pix.size();
            updateImageSize();
        }
    }

    void MediaWidget::setFilepath(const QString path)
    {
        m_filepath = path;
        setToolTip(path);
    }

    void MediaWidget::setText(const QString text)
    {
        m_hasImg = false;
        imageLabel->setText(text);
    }

    void MediaWidget::setContextMenu(QMenu* menu)
    {
        m_menu = menu;
    }

    QString MediaWidget::filepath() const
    {
        return m_filepath;
    }

    void MediaWidget::showPlayer(bool value)
    {
        playerWidget->setVisible(value);
    }

    void MediaWidget::contextMenuEvent(QContextMenuEvent *event)
    {
        if(m_menu)
            m_menu->exec(event->globalPos());
    }

    void MediaWidget::updateImageSize()
    {
        if(!m_hasImg)
            return;

        int width=0, height=0;
        double ratio = m_size.width() / (double)m_size.height();
        double ratioW = this->width() / (double)this->height();

        if((ratio>1 && ratio>ratioW) || (ratio<1 && ratio>ratioW))
        {
            width = this->width();
            height = (int)(width / ratio);
        }
        else
        {
            height = this->height();
            width = (int)(height * ratio);
        }

        imageLabel->setMaximumWidth(width);
        imageLabel->setMaximumHeight(height);
        imageLabel->setMinimumWidth(width);
        imageLabel->setMinimumHeight(height);

        int margin = (this->width()-width)/2;
        verticalLayout->setContentsMargins(margin, 0, 0, 0);
    }

    void MediaWidget::resizeEvent(QResizeEvent* event)
    {
        Q_UNUSED(event);
        updateImageSize();
    }

    void MediaWidget::playPressed()
    {
        emit play();
    }

    void MediaWidget::stopPressed()
    {
        emit stop();
    }

}
