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

#ifndef OPTIONDIALOG_H_
#define OPTIONDIALOG_H_

#include <QtGui>
#include <image.h>
#include <config.h>
#include <logger.h>

#include "ui_optiondialog.h"
#include "modulemanager.h"

using namespace SilentEyeFramework;

namespace SilentEye {

    //! Mother class for encode and decode dialog.
    class OptionDialog : public QDialog, public Ui::OptionDialog
    {
        Q_OBJECT;

    protected:
        QPointer<Logger> m_logger;

        QPointer<Media> m_md;
        QString m_filePath;
        QWidget* m_optionWidget;

        bool m_checkSize;
        int m_originalHeight;

    public:
        OptionDialog(QWidget* parent=0);
        ~OptionDialog();

        void setMedia(QPointer<Media>);

    protected:
        void setupDialog();
        void updateDialogSize();
        void loadFormats();
        void loadEncryptionTypes();
        bool isDialogReady();
        bool checkCryptoPassword();
        FormatModuleInterface* currentFormatModule();
        CryptoModuleInterface* currentCryptoModule();
        Data::DataFormat currentCharset();
        int dataSizeRemaining(FormatModuleInterface* module = NULL, QPointer<Media> = QPointer<Media>());
        void displayException(const QString& title, const SilentEyeException e);
        virtual QWidget* currentFormatWidget(FormatModuleInterface*) = 0;

    private:
        void connectSignals();

    protected slots:
        virtual void ok() = 0;
        void rejected();
        void selectFolder();
        void selectExportFolder();
        void selectFile();
        void addFile(const QString&);
        void removeFile();
        void formatChanged(const QString&);
        void cryptoStatusChanged(int);
        void cryptoPwd1TextEdited(QString);
        void cryptoPwd2TextEdited(QString);
        void compressionModeChange(int);
        void valueChanged();
    };
}
#endif
