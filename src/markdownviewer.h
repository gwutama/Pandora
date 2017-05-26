#ifndef MARKDOWNVIEWER_H
#define MARKDOWNVIEWER_H

#include <QWidget>
#include <QWebEngineView>
#include <QUrl>
#include <QSharedPointer>
#include "appconfig.h"
#include "documentgenerator.h"

/**
 * @brief The MarkdownViewer class
 *
 * Resource:
 * http://doc.qt.io/qt-5/qtwidgets-richtext-syntaxhighlighter-example.html
 */
class MarkdownViewer : public QWidget
{
    Q_OBJECT

public:
    explicit MarkdownViewer(QSharedPointer<AppConfig> config,
                            QWidget* parent = 0);
    virtual ~MarkdownViewer();

public slots:
    void close();
    void load();

private slots:
    void onScrollPositionChanged(const QPointF pos);
    void onPageLoaded(bool ok);

private:
    void initDocumentGenerator();
    void loadPage(const QString& path);

private:
    QSharedPointer<AppConfig> mConfig;
    QWebEngineView* mWebView;
    QSharedPointer<DocumentGenerator> mGenerator;
    QPointF mScrollPos;

    static const char* sTag;
};

#endif // MARKDOWNVIEWER_H
