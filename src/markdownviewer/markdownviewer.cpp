#include "markdownviewer.h"
#include <QDebug>
#include <QHBoxLayout>

const char* MarkdownViewer::sTag = "[MarkdownViewer]";

MarkdownViewer::MarkdownViewer(QSharedPointer<AppConfig> config,
                               QWidget* parent) :
    QWidget(parent),
    mConfig(config)
{
    // Setup web engine
    mWebView = new QWebEngineView(this);
    connect(mWebView->page(), &QWebEnginePage::scrollPositionChanged,
            this, &MarkdownViewer::onScrollPositionChanged);
    connect(mWebView, &QWebEngineView::loadFinished, this, &MarkdownViewer::onPageLoaded);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(mWebView);
    setLayout(layout);

    close();
}


MarkdownViewer::~MarkdownViewer()
{
}


void MarkdownViewer::close()
{
    QUrl url("qrc:/markdownviewer/emptymarkdownviewer.html");
    mWebView->load(url);
    mScrollPos = QPointF();
}


bool MarkdownViewer::load()
{
    // Directly generate the html so that we have something to display on the screen
    initDocumentGenerator();
    return mGenerator->generate(mConfig->markdownFile());
}


void MarkdownViewer::initDocumentGenerator()
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

    connect(mGenerator.data(), &DocumentGenerator::generated, this, &MarkdownViewer::loadPage,
            Qt::UniqueConnection);
}


void MarkdownViewer::loadPage(const QString& path)
{
    qDebug() << sTag << "Loading page" << path;
    QUrl url("file://" + path);
    mWebView->load(url);
}


void MarkdownViewer::onScrollPositionChanged(const QPointF pos)
{
    mScrollPos = pos;
}


void MarkdownViewer::onPageLoaded(bool ok)
{
    if (ok)
    {
        // Set scroll position to the last position
        QString scrollJs("window.scrollTo(%1, %2);");
        scrollJs = scrollJs.arg(mScrollPos.x()).arg(mScrollPos.y());
        mWebView->page()->runJavaScript(scrollJs);
    }
}
