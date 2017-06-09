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
    void showContextMenu(const QStringList& suggestions,
                         const QPoint& point);

    void increaseFontSize();
    void decreaseFontSize();
    void setMargin(unsigned int size);

private slots:
    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void onSuggestionActionTriggered();
    void checkVerticalScroll();
    void checkTextChanged();
    void replaceSelection(const QString& replacement);

signals:
    void laxVerticalScrollEnd(int position);
    void laxTextChanged(const QString& text);

private:
    void setup();
    void indent(QKeyEvent* event);
    void outdent(QKeyEvent* event);

private:
    QSharedPointer<MarkdownEditorHighlighter> mHighlighter;
    QList<QAction*> mSuggestionActions;
    QTimer mVerticalScrollTimer;
    QTimer mContentChangeTimer;
    int mVerticalScrollPos;
    QString mOldContent;
    static const char* sTag;
};

#endif // MARKDOWNTEXTEDIT_H
