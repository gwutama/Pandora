#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>

const char* MainWindow::sTag = "[MainWindow]";

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mConfig(QSharedPointer<AppConfig>(new AppConfig))
{
    // Setup ui
    mUi->setupUi(this);
    setAccessibleName("Pandora");
    setWindowTitle("Pandora");

    // Setup markdown editor
    mEditor = new MarkdownEditor(mConfig, this);
    mEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mUi->horizontalLayout->addWidget(mEditor);

    // Setup markdown viewer
    mViewer = new MarkdownViewer(mConfig, this);
    mViewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mUi->horizontalLayout->addWidget(mViewer);

    connect(mEditor, &MarkdownEditor::contentChanged, mViewer, &MarkdownViewer::load);
    connect(mViewer, &MarkdownViewer::loaded, mEditor, &MarkdownEditor::refocusEditor);

    // Setup preferences dialog
    mCfgDialog = new PreferencesDialog(mConfig, this);
    connect(mUi->actionPreferences, &QAction::triggered, mCfgDialog, &PreferencesDialog::show);
    connect(mCfgDialog, &PreferencesDialog::saved, mEditor, &MarkdownEditor::open);

    // Menu signals slots
    connect(mUi->actionNew, &QAction::triggered, this, &MainWindow::onNewActionTriggered);
    connect(mUi->actionOpen, &QAction::triggered, this, &MainWindow::onOpenActionTriggered);
    connect(mUi->actionClose, &QAction::triggered, this, &MainWindow::onCloseActionTriggered);
    connect(mUi->actionSave, &QAction::triggered, this, &MainWindow::onSaveActionTriggered);
    connect(mUi->actionSaveAs, &QAction::triggered, this, &MainWindow::onSaveAsActionTriggered);
    connect(mUi->actionFindReplace, &QAction::triggered,
            mEditor, &MarkdownEditor::showFindReplaceWidget);
    connect(mUi->actionTogglePreview, &QAction::triggered,
            this, &MainWindow::onTogglePreviewActionTriggered);

    // Set window size and position
    setMinimumSize(800, 800);
    restoreGeometry(mConfig->settings().value("geometry").toByteArray());
    restoreState(mConfig->settings().value("state").toByteArray());

    uiStateNoFileOpen();
}


MainWindow::~MainWindow()
{
    delete mUi;
}


void MainWindow::uiStateNoFileOpen()
{
    // Disable menus File->Close, File->Save, File->Save As
    // Disable editor
    mUi->actionClose->setDisabled(true);
    mUi->actionSave->setDisabled(true);
    mUi->actionSaveAs->setDisabled(true);
    mUi->menuEdit->setDisabled(true);
    mEditor->setDisabled(true);
}


void MainWindow::uiStateFileOpened()
{
    mUi->actionClose->setEnabled(true);
    mUi->actionSave->setEnabled(true);
    mUi->actionSaveAs->setEnabled(true);
    mUi->menuEdit->setEnabled(true);
    mEditor->setEnabled(true);
}


void MainWindow::closeEvent(QCloseEvent* /*event*/)
{
    // Save window size and position
    mConfig->settings().setValue("geometry", saveGeometry());
    mConfig->settings().setValue("state", saveState());
    mConfig->settings().sync();
}


void MainWindow::onNewActionTriggered(bool /*checked*/)
{
    mViewer->close();
    mEditor->close();

    // Create new tmp file and load it
    if (!mTmpMarkdownFile.isNull())
    {
        mTmpMarkdownFile.clear();
    }

    mTmpMarkdownFile = QSharedPointer<QTemporaryFile>(new QTemporaryFile);

    if (!mTmpMarkdownFile->open())
    {
        qWarning() << sTag << "Error creating temporary file";
        return;
    }

    openFileHelper(mTmpMarkdownFile->fileName());
}


void MainWindow::onOpenActionTriggered(bool /*checked*/)
{
    QString file = QFileDialog::getOpenFileName(this, tr("Markdown File"), "",
                                                "Markdown file (*.md)");

    if (file.isEmpty())
    {
        qWarning() << sTag << "No file selected";
        return;
    }

    openFileHelper(file);
}


void MainWindow::onCloseActionTriggered(bool /*checked*/)
{
    closeFileHelper();
}


void MainWindow::onSaveActionTriggered(bool checked)
{
    // If we are saving a "new" file, then the user must specify where he wants to save the file.
    // Otherwise, just save the file.
    if (!mTmpMarkdownFile.isNull())
    {
        onSaveAsActionTriggered(checked);
    }
    else
    {
        mEditor->save();
    }
}


void MainWindow::onSaveAsActionTriggered(bool /*checked*/)
{
    QString file = QFileDialog::getSaveFileName(this, tr("Save Markdown File As..."), "",
                                                "Markdown file (*.md)");

    if (file.isEmpty())
    {
        qWarning() << sTag << "No file selected";
        return;
    }

    // open the just saved file and remove temporary file (if any)
    if (mEditor->saveAs(file))
    {
        openFileHelper(file);

        if (!mTmpMarkdownFile.isNull())
        {
            mTmpMarkdownFile.clear();
        }
    }
}


bool MainWindow::openFileHelper(const QString& file)
{
    if (file.isEmpty())
    {
        qWarning() << sTag << "No file selected";
        return false;
    }

    QFileInfo finfo(file);

    if (!finfo.isFile() || !finfo.isReadable() || !finfo.isWritable())
    {
        qWarning() << sTag << "File is not readable and/or writable" << file;
        return false;
    }

    mConfig->setMarkdownFile(file);
    mConfig->save();

    if (!mEditor->open())
    {
        qWarning() << sTag << "Cannot load file:" << file;
        return false;
    }

    qDebug() << sTag << "File successfully opened:" << file;
    uiStateFileOpened();
    return true;
}


void MainWindow::closeFileHelper()
{
    mViewer->close();
    mEditor->close();
    uiStateNoFileOpen();
}


void MainWindow::onTogglePreviewActionTriggered(bool /*checked*/)
{
    if (mViewer->isVisible())
    {
        mViewer->hide();
        mViewer->setDisabled(true);
        mEditor->setWideMargin();
    }
    else
    {
        mViewer->show();
        mViewer->setEnabled(true);
        mEditor->setNarrowMargin();

        // Directly load preview
        mViewer->load(mEditor->content());
    }
}
