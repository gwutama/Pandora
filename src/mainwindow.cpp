#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <easylogging++.h>
#include "processrunner.h"
#include <QSysInfo>

const char* MainWindow::sTag = "[MainWindow]";

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mConfig(QSharedPointer<AppConfig>(new AppConfig))
{
    if (QSysInfo::productType() == "macos")
    {
        putenv((char*) "PATH=/usr/local/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin");
    }

    // Setup ui
    mUi->setupUi(this);
    setAccessibleName("Pandoc Viewer");
    setWindowTitle("Pandoc Viewer");

    // Setup web engine
    mWebView = new QWebEngineView(this);
    mUi->verticalLayout->addWidget(mWebView);
    connect(mWebView->page(), &QWebEnginePage::scrollPositionChanged,
            this, &MainWindow::onScrollPositionChanged);
    connect(mWebView, &QWebEngineView::loadFinished, this, &MainWindow::onPageLoaded);

    // Set window size and position
    setMinimumSize(600, 800);
    restoreGeometry(mConfig->settings().value("geometry").toByteArray());
    restoreState(mConfig->settings().value("state").toByteArray());

    // Setup preferences dialog and show it on program start
    mCfgDialog = new PreferencesDialog(mConfig, this);
    connect(mUi->actionPreferences, &QAction::triggered, mCfgDialog, &PreferencesDialog::show);
    connect(mCfgDialog, &PreferencesDialog::saved, this, &MainWindow::generate);
}


MainWindow::~MainWindow()
{
    delete mUi;
}


void MainWindow::generate()
{
    // Directly generate the html so that we have something to display on the screen
    initDocumentGenerator();
    mGenerator->generate(mConfig->markdownFile());
}


void MainWindow::initDocumentGenerator()
{
    // Setup document generator component
    mGenerator = QSharedPointer<DocumentGenerator>(new DocumentGenerator(mConfig->markdownFile()));
    mGenerator->setBibtexFile(mConfig->bibtexFile());

    if (mConfig->outputFile())
    {
        mGenerator->setOutputFile(mConfig->outputFile()->fileName());
    }

    if (mConfig->cssFile())
    {
        mGenerator->setCssFile(mConfig->cssFile()->fileName());
    }

    connect(mGenerator.data(), &DocumentGenerator::generated, this, &MainWindow::loadPage,
            Qt::UniqueConnection);
}


void MainWindow::loadPage(const QString& path)
{
    LOG(DEBUG) << sTag << "Loading page" << path;
    QUrl url("file://" + path);
    mWebView->load(url);
}


void MainWindow::closeEvent(QCloseEvent* /*event*/)
{
    mConfig->settings().setValue("geometry", saveGeometry());
    mConfig->settings().setValue("state", saveState());
    mConfig->settings().sync();
}


void MainWindow::onScrollPositionChanged(const QPointF pos)
{
    mScrollPos = pos;
}


void MainWindow::onPageLoaded(bool ok)
{
    if (ok)
    {
        // Set scroll position to the last position
        QString scrollJs("window.scrollTo(%1, %2);");
        scrollJs = scrollJs.arg(mScrollPos.x()).arg(mScrollPos.y());
        mWebView->page()->runJavaScript(scrollJs);
    }
}
