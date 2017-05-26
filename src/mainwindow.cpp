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
    connect(mCfgDialog, &PreferencesDialog::saved, mViewer, &MarkdownViewer::generate);
    connect(mCfgDialog, &PreferencesDialog::saved, mEditor, &MarkdownEditor::load);

    // Menu signals slots
    connect(mUi->actionOpen, &QAction::triggered, this, &MainWindow::onOpenFileActionTriggered);
    connect(mUi->actionClose, &QAction::triggered, this, &MainWindow::onCloseFileActionTriggered);

    // Set window size and position
    setMinimumSize(800, 800);
    restoreGeometry(mConfig->settings().value("geometry").toByteArray());
    restoreState(mConfig->settings().value("state").toByteArray());
}


MainWindow::~MainWindow()
{
    delete mUi;
}


void MainWindow::closeEvent(QCloseEvent* /*event*/)
{
    // Save window size and position
    mConfig->settings().setValue("geometry", saveGeometry());
    mConfig->settings().setValue("state", saveState());
    mConfig->settings().sync();
}


void MainWindow::onOpenFileActionTriggered(bool /*checked*/)
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
    mViewer->generate();
    mEditor->load();
}


void MainWindow::onCloseFileActionTriggered(bool checked)
{
    mViewer->clear();
    mEditor->clear();
}
