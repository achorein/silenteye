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
//  along with SilentEye. If not, see <http://www.gnu.org/licenses/>.

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtGui>
#include "ui_mainwindow.h"

#include "aboutdialog.h"
#include "encodedialog.h"
#include "propertydialog.h"
#include "preferencedialog.h"
#include "updatedialog.h"
#include "decodedialog.h"
#include "mediawidget.h"
#include "urldialog.h"

using namespace SilentEyeFramework;

//! SilentEye application GUI
namespace SilentEye {

    //! The main window of SilentEye
    class MainWindow : public QMainWindow, public Ui::MainWindow
    {
        Q_OBJECT;

    private:
        EncodeDialog encodeDialog;
        PropertyDialog propertyDialog;
        DecodeDialog decodeDialog;
        PreferenceDialog preferenceDialog;
        AboutDialog aboutDialog;
        UpdateDialog updateDialog;
        UrlDialog urlDialog;

        QMenu* contextMenu;
        QPushButton* closeTabButton;

        QMap<QString, QPointer<Media> > m_mediaMap;
        bool m_hasMediaLoaded;

        QPointer<QSound> m_currentSound;

        QPointer<Logger> m_logger;
    public:
        MainWindow(QString url = "", QWidget *parent = 0);
        ~MainWindow();

    private:
        void connectSignals();
        void setEnabledImageActions(const bool value);
        QPointer<Media> currentMedia();
        void closeTabByPath(QString);

    private slots:
        void dragEnterEvent(QDragEnterEvent*);
        void dropEvent(QDropEvent*);
        void closeCurrentTab();
        void addMediaTab(QPointer<Media>);

        void paste();
        void copy();
        void execDecodeDialog();
        void execEncodeDialog();
        void execPropertyDialog();
        void execUpdateDialog();
        void execUrlDialog();

        void newMedia(QString);
        void openFile();

        void playMedia();
        void stopMedia();

        void httpError(QString, QString);
    };
}

#endif
