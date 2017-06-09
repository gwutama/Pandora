#ifndef MARKDOWNTEXTEDIT_H
#define MARKDOWNTEXTEDIT_H

#include <QPlainTextEdit>
#include <QStringList>
#include <QTimer>
#include "markdowneditorhighlighter.h"
#include "spellcheck.h"

class MarkdownTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit MarkdownTextEdit(QWidget* parent = 0);
    virtual ~MarkdownTextEdit();

    inline void setSpellCheck(QSharedPointer<SpellCheck> spellCheck)
    { mSpellCheck = spellCheck; }

public slots:
    void increaseFontSize();
    void decreaseFontSize();
    void setMargin(unsigned int size);
    void spellcheckVisibleText();

private slots:
    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void onSuggestionActionTriggered();
    void checkVerticalScroll();
    void checkTextChanged();
    void replaceSelection(const QString& replacement);
    void showContextMenu(const QPoint& point);
    void showContextMenuWithSuggestions(const QPoint& point,
                                        const QStringList& suggestions = QStringList());

private:
    void setup();
    void indent(QKeyEvent* event);
    void outdent(QKeyEvent* event);

signals:
    void laxVerticalScrollEnd(int position);
    void laxTextChanged(const QString& text);

private:
    QSharedPointer<MarkdownEditorHighlighter> mHighlighter;
    QSharedPointer<SpellCheck> mSpellCheck;

    QList<QAction*> mSuggestionActions;
    QTimer mVerticalScrollTimer;
    QTimer mContentChangeTimer;
    int mVerticalScrollPos;
    QString mOldContent;
    QList<QTextEdit::ExtraSelection> mSpellCheckSelections;

    static const char* sTag;
};

#endif // MARKDOWNTEXTEDIT_H
