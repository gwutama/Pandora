#ifndef MARKDOWNEDITOR_H
#define MARKDOWNEDITOR_H

#include <QWidget>
#include <QSharedPointer>
#include "appconfig.h"
#include "markdowneditorhighlighter.h"

namespace Ui
{
class MarkdownEditor;
}

/**
 * @brief The MarkdownEditor class
 * Resources:
 * http://doc.qt.io/qt-5/qtwidgets-richtext-syntaxhighlighter-example.html
 */
class MarkdownEditor : public QWidget
{
    Q_OBJECT

public:
    explicit MarkdownEditor(QSharedPointer<AppConfig> config,
                            QWidget* parent = 0);
    ~MarkdownEditor();

public slots:
    void load();
    void newFile();
    void openFile(const QString& path = QString());

private:
    void setupEditor();

private:
    Ui::MarkdownEditor* ui;
    QSharedPointer<AppConfig> mConfig;
    MarkdownEditorHighlighter* mHighlighter;
};

#endif // MARKDOWNEDITOR_H
