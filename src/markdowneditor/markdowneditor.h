#ifndef MARKDOWNEDITOR_H
#define MARKDOWNEDITOR_H

#include <QWidget>
#include <QSharedPointer>
#include <QTimer>
#include <QTextEdit>
#include "common/appconfig.h"
#include "markdowneditorhighlighter.h"
#include "findreplacewidget.h"

namespace Ui
{
class MarkdownEditor;
}

/**
 * @brief The MarkdownEditor class
 * Resources:
 * http://doc.qt.io/qt-5/qtwidgets-richtext-syntaxhighlighter-example.html
 * http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html
 */
class MarkdownEditor : public QWidget
{
    Q_OBJECT

public:
    explicit MarkdownEditor(QSharedPointer<AppConfig> config,
                            QWidget* parent = 0);
    ~MarkdownEditor();

public slots:
    void refocusEditor();
    bool open();
    void close();
    bool save();
    bool saveAs(const QString& path);
    void showFindReplaceWidget();

signals:
    void contentChanged(const QString& contents);

private slots:
    void onEscKeyActivated();
    void checkContentChanged();
    void highlightFoundText(const QString& searchString);
    void removeHighlightFoundText();
    void goToNextFoundText();
    void goToPreviousFoundText();

private:
    void setupEditor();
    bool openFile(const QString& path = QString());

private:
    Ui::MarkdownEditor* mUi;
    QSharedPointer<AppConfig> mConfig;
    QSharedPointer<MarkdownEditorHighlighter> mHighlighter;
    QTimer mContentChangeTimer;
    QString mOldContent;
    FindReplaceWidget* mFindReplaceWidget;
    QList<QTextEdit::ExtraSelection> mFoundTextSelections;

    static const char* sTag;
};

#endif // MARKDOWNEDITOR_H