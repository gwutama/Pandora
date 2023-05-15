#ifndef MARKDOWNTEXTEDIT_H
#define MARKDOWNTEXTEDIT_H

#include <QPlainTextEdit>
#include <QStringList>
#include <QTimer>
#include "markdowneditorhighlighter.h"

class MarkdownTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit MarkdownTextEdit(QWidget* parent = 0);
    virtual ~MarkdownTextEdit();

public slots:
    void increaseFontSize();
    void decreaseFontSize();
    void setMargin(unsigned int size);

private slots:
    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void checkVerticalScroll();
    void checkTextChanged();
    void checkMouseHoverPositionChanged();

private:
    void indent(QKeyEvent* event);
    void outdent(QKeyEvent* event);

signals:
    void laxVerticalScrollEnd(int position);
    void laxTextChanged(const QString& text);
    void mouseHover(const QTextCursor& cursor);

private:
    QSharedPointer<MarkdownEditorHighlighter> mHighlighter;
    QTimer mVerticalScrollTimer;
    QTimer mContentChangeTimer;
    QTimer mHoverCursorTimer;
    int mVerticalScrollPos;
    QString mOldContent;
    QTextCursor mHoverCursor;

    static const char* sTag;
};

#endif // MARKDOWNTEXTEDIT_H
