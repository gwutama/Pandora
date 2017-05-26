#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
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

    // Setup preferences dialog
    mCfgDialog = new PreferencesDialog(mConfig, this);
    connect(mUi->actionPreferences, &QAction::triggered, mCfgDialog, &PreferencesDialog::show);
    connect(mCfgDialog, &PreferencesDialog::saved, mViewer, &MarkdownViewer::load);
    connect(mCfgDialog, &PreferencesDialog::saved, mEditor, &MarkdownEditor::open);

    // Menu signals slots
    connect(mUi->actionOpen, &QAction::triggered, this, &MainWindow::onOpenActionTriggered);
    connect(mUi->actionClose, &QAction::triggered, this, &MainWindow::onCloseActionTriggered);
    connect(mUi->actionSave, &QAction::triggered, this, &MainWindow::onSaveActionTriggered);

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
    // Disable menus File->Close, File->Save
    // Disable editor
    mUi->actionClose->setDisabled(true);
    mUi->actionSave->setDisabled(true);
    mEditor->setDisabled(true);
}


void MainWindow::uiStateFileOpened()
{
    mUi->actionClose->setEnabled(true);
    mUi->actionSave->setEnabled(true);
    mEditor->setEnabled(true);
}


void MainWindow::closeEvent(QCloseEvent* /*event*/)
{
    // Save window size and position
    mConfig->settings().setValue("geometry", saveGeometry());
    mConfig->settings().setValue("state", saveState());
    mConfig->settings().sync();
}


void MainWindow::onNewActionTriggered(bool checked)
{

}


void MainWindow::onOpenActionTriggered(bool /*checked*/)
{
    QString file = QFileDialog::getOpenFileName(this, tr("Markdown File"), "",
                                                "Markdown file (*.md *.txt *.text)");

    if (file.isEmpty())
    {
        qWarning() << sTag << "No file selected";
        return;
    }

    mConfig->setMarkdownFile(file);
    mConfig->save();

    if (mViewer->load() && mEditor->open())
    {
        qDebug() << sTag << "File successfully opened";
        uiStateFileOpened();
    }
    else
    {
        qWarning() << sTag << "Error opening file";
    }
}


void MainWindow::onCloseActionTriggered(bool checked)
{
    mViewer->close();
    mEditor->close();
    uiStateNoFileOpen();
}


void MainWindow::onSaveActionTriggered(bool checked)
{
    mEditor->save();
}
