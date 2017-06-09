#ifndef SPELLCHECKACTIONSDELEGATE_H
#define SPELLCHECKACTIONSDELEGATE_H

#include "spellcheck.h"
#include "markdowntextedit.h"

class SpellCheckActionsDelegate : public QObject
{
    Q_OBJECT

public:
    explicit SpellCheckActionsDelegate(MarkdownTextEdit* textEdit,
                                       QSharedPointer<SpellCheck> spellCheck,
                                       QObject* parent = nullptr);
    virtual ~SpellCheckActionsDelegate();

    inline QList<QTextEdit::ExtraSelection> selections()
    { return mSelections; }

public slots:
    void checkVisibleText();

private slots:
    void showContextMenu(const QPoint& point);
    void showContextMenuWithSuggestions(const QPoint& point,
                                        const QStringList& suggestions = QStringList());
    void replaceSelectionWithTextReplacement();

private:
    MarkdownTextEdit* mTextEdit;
    QSharedPointer<SpellCheck> mSpellCheck;
    QList<QTextEdit::ExtraSelection> mSelections;
    QList<QAction*> mSuggestionActions;

    static const char* sTag;
};

#endif // SPELLCHECKACTIONSDELEGATE_H
