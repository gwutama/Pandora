#ifndef MARKDOWNVIEWER_H
#define MARKDOWNVIEWER_H

#include <QWidget>
#include <QWebEngineView>
#include <QUrl>
#include <QSharedPointer>
#include "common/appconfig.h"
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
    void load(const QString& content);

private slots:
    void onScrollPositionChanged(const QPointF pos);
    void onPageLoaded(bool ok);

signals:
    void loaded();

private:
    void initDocumentGenerator();
    void loadPageInWebView(const QString& path);

private:
    QSharedPointer<AppConfig> mConfig;
    QWebEngineView* mWebView;
    QSharedPointer<DocumentGenerator> mGenerator;
    QPointF mScrollPos;

    static const char* sTag;
};

#endif // MARKDOWNVIEWER_H
