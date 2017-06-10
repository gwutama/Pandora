#ifndef SPELLCHECKACTIONSDELEGATE_H
#define SPELLCHECKACTIONSDELEGATE_H

#include "proofreadactionsdelegatebase.h"
#include "spellcheck.h"
#include "markdowntextedit.h"

class SpellCheckActionsDelegate : public ProofreadActionsDelegateBase
{
    Q_OBJECT

public:
    explicit SpellCheckActionsDelegate(MarkdownTextEdit* textEdit,
                                       QSharedPointer<SpellCheck> spellCheck,
                                       QObject* parent = nullptr);
    virtual ~SpellCheckActionsDelegate();

private slots:
    virtual void runVisibleTextCheck(const QString& text, int startPos, int endPos);
    virtual void runDocumentCheck(const QString& text);
    virtual void showContextMenu(const QPoint& point);
    virtual void replaceSelectionWithTextReplacement();

private:
    QSharedPointer<SpellCheck> mSpellCheck;
    static const char* sTag;
};

#endif // SPELLCHECKACTIONSDELEGATE_H
