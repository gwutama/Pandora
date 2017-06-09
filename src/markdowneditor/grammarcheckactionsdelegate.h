#ifndef GRAMMARCHECKACTIONSDELEGATE_H
#define GRAMMARCHECKACTIONSDELEGATE_H

#include "proofreadactionsdelegatebase.h"
#include "languagetool.h"

class GrammarCheckActionsDelegate : public ProofreadActionsDelegateBase
{
    Q_OBJECT

public:
    explicit GrammarCheckActionsDelegate(MarkdownTextEdit* textEdit,
                                         QSharedPointer<LanguageTool> languageTool,
                                         QObject* parent = nullptr);
    virtual ~GrammarCheckActionsDelegate();

private slots:
    virtual void runVisibleTextCheck(const QString& text, int startPos, int endPos);
    virtual void runDocumentCheck(const QString& text);
    void checkFinished();
    virtual void showContextMenu(const QPoint& point);
    virtual void replaceSelectionWithTextReplacement();

private:
    QSharedPointer<LanguageTool> mLanguageTool;
    int mCheckOffset;
    QList<LanguageToolMatch> mMatches;
    LanguageToolMatch mSelectedMatch;
    static const char* sTag;
};

#endif // GRAMMARCHECKACTIONSDELEGATE_H
