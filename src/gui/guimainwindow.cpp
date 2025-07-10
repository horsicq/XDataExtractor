/* Copyright (c) 2025 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "guimainwindow.h"

#include "ui_guimainwindow.h"

GuiMainWindow::GuiMainWindow(QWidget *pParent) : QMainWindow(pParent), ui(new Ui::GuiMainWindow)
{
    ui->setupUi(this);

    g_pFile = nullptr;
    g_pXInfo = nullptr;

    setWindowTitle(XOptions::getTitle(X_APPLICATIONDISPLAYNAME, X_APPLICATIONVERSION));

    setAcceptDrops(true);

    g_xOptions.setName(X_OPTIONSFILE);

    g_xOptions.addID(XOptions::ID_VIEW_STAYONTOP, false);
    g_xOptions.addID(XOptions::ID_VIEW_STYLE, "Fusion");
    g_xOptions.addID(XOptions::ID_VIEW_LANG, "System");
    g_xOptions.addID(XOptions::ID_VIEW_QSS, "");
    g_xOptions.addID(XOptions::ID_VIEW_FONT_CONTROLS, XOptions::getDefaultFont().toString());
    g_xOptions.addID(XOptions::ID_VIEW_FONT_TABLEVIEWS, XOptions::getMonoFont().toString());
    g_xOptions.addID(XOptions::ID_VIEW_FONT_TREEVIEWS, XOptions::getDefaultFont().toString());
    g_xOptions.addID(XOptions::ID_VIEW_FONT_TEXTEDITS, XOptions::getMonoFont().toString());
    g_xOptions.addID(XOptions::ID_FILE_SAVELASTDIRECTORY, true);
    g_xOptions.addID(XOptions::ID_FILE_SAVERECENTFILES, true);

#ifdef Q_OS_WIN
    g_xOptions.addID(XOptions::ID_FILE_CONTEXT, "*");
#endif

    // NFDOptionsWidget::setDefaultValues(&g_xOptions);

    g_xOptions.load();

    g_xShortcuts.setName(X_SHORTCUTSFILE);
    g_xShortcuts.setNative(g_xOptions.isNative());

    g_xShortcuts.addGroup(XShortcuts::GROUPID_HEX);
    g_xShortcuts.addGroup(XShortcuts::GROUPID_DISASM);
    g_xShortcuts.addGroup(XShortcuts::GROUPID_TABLE);

    g_xShortcuts.load();

    connect(&g_xOptions, SIGNAL(openFile(QString)), this, SLOT(setFileName(QString)));

    g_pRecentFilesMenu = g_xOptions.createRecentFilesMenu(this);

    ui->toolButtonRecentFiles->setEnabled(g_xOptions.getRecentFiles().count());

    ui->widgetMain->setGlobal(&g_xShortcuts, &g_xOptions);

    adjustView();

    if (QCoreApplication::arguments().count() > 1) {
        setFileName(QCoreApplication::arguments().at(1));
    }
}

GuiMainWindow::~GuiMainWindow()
{
    if (g_pXInfo) {
        delete g_pXInfo;
        g_pXInfo = nullptr;
    }

    if (g_pFile) {
        if (g_pFile->isOpen()) {
            g_pFile->close();
        }

        delete g_pFile;
        g_pFile = nullptr;
    }

    g_xOptions.save();
    g_xShortcuts.save();

    delete ui;
}

void GuiMainWindow::setFileName(const QString &sName)
{
    QFileInfo fi(sName);

    if (fi.isFile()) {
        ui->lineEditFileName->setText(sName);

        if (g_pXInfo) {
            delete g_pXInfo;
            g_pXInfo = nullptr;
        }

        if (g_pFile) {
            if (g_pFile->isOpen()) {
                g_pFile->close();
            }

            delete g_pFile;
            g_pFile = nullptr;
        }

        g_pFile = new QFile;
        g_pXInfo = new XInfoDB;

        g_pFile->setFileName(sName);

        if (g_pFile->open(QIODevice::ReadOnly)) {
            XExtractor::OPTIONS extractorOptions = XExtractor::getDefaultOptions();
            extractorOptions.bMenu_Hex = true;

            ui->widgetMain->setData(g_pFile, g_pXInfo, extractorOptions, true);
            g_xOptions.setLastFileName(sName);

            adjustView();
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Cannot open file"));
        }
    }
}

void GuiMainWindow::on_pushButtonExit_clicked()
{
    this->close();
}

void GuiMainWindow::on_pushButtonOpenFile_clicked()
{
    QString sDirectory = g_xOptions.getLastDirectory();

    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open file") + QString("..."), sDirectory, tr("All files") + QString(" (*)"));

    if (!sFileName.isEmpty()) {
        if (g_xOptions.isScanAfterOpen()) {
            setFileName(sFileName);
        }
    }
}

void GuiMainWindow::on_pushButtonAbout_clicked()
{
    DialogAbout di(this);

    di.exec();
}

void GuiMainWindow::dragEnterEvent(QDragEnterEvent *pEvent)
{
    pEvent->acceptProposedAction();
}

void GuiMainWindow::dragMoveEvent(QDragMoveEvent *pEvent)
{
    pEvent->acceptProposedAction();
}

void GuiMainWindow::dropEvent(QDropEvent *pEvent)
{
    const QMimeData *mimeData = pEvent->mimeData();

    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();

        if (urlList.count()) {
            QString sFileName = urlList.at(0).toLocalFile();

            sFileName = XBinary::convertFileName(sFileName);

            setFileName(sFileName);
        }
    }
}

void GuiMainWindow::on_pushButtonOptions_clicked()
{
    DialogOptions dialogOptions(this, &g_xOptions, XOptions::GROUPID_FILE);
    dialogOptions.setGlobal(&g_xShortcuts, &g_xOptions);

    dialogOptions.exec();

    adjustView();
}

void GuiMainWindow::adjustView()
{
    if (g_xOptions.isIDPresent(XOptions::ID_VIEW_STAYONTOP)) {
        g_xOptions.adjustStayOnTop(this);
    }

    g_xOptions.adjustWidget(this, XOptions::ID_VIEW_FONT_CONTROLS);
}

void GuiMainWindow::on_toolButtonRecentFiles_clicked()
{
    g_pRecentFilesMenu->exec(QCursor::pos());

    ui->toolButtonRecentFiles->setEnabled(g_xOptions.getRecentFiles().count());
}

void GuiMainWindow::on_lineEditFileName_textChanged(const QString &sString)
{
    // XFormats::setFileTypeComboBox(XBinary::FT_UNKNOWN, sString, ui->comboBoxType);
}
