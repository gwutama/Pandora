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
}


MarkdownViewer::~MarkdownViewer()
{
    if (!mGenerator.isNull())
    {
        mGenerator.clear();
    }
}


void MarkdownViewer::close()
{
    QUrl url("qrc:/markdownviewer/emptymarkdownviewer.html");
    mWebView->load(url);
    mScrollPos = QPointF();

    if (!mGenerator.isNull())
    {
        mGenerator.clear();
    }
}


void MarkdownViewer::load(const QString& content)
{
    // Ignore preview generation if widget is disabled
    if (!isEnabled())
    {
        return;
    }

    // Directly generate the html so that we have something to display on the screen
    if (mGenerator.isNull())
    {
        initDocumentGenerator();
    }

    mGenerator->setContent(content);
    mGenerator->generate();
}


void MarkdownViewer::initDocumentGenerator()
{
    // Setup document generator component
    mGenerator = QSharedPointer<DocumentGenerator>(new DocumentGenerator);
    mGenerator->setBibtexFile(mConfig->bibtexFile());

    if (mConfig->outputFile())
    {
        mGenerator->setOutputFile(mConfig->outputFile()->fileName());
    }

    if (mConfig->cssFile())
    {
        mGenerator->setCssFile(mConfig->cssFile()->fileName());
    }

    connect(mGenerator.data(), &DocumentGenerator::generated,
            this, &MarkdownViewer::loadPageInWebView,
            Qt::UniqueConnection);
}


void MarkdownViewer::loadPageInWebView(const QString& path)
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
        emit loaded();
    }
}
