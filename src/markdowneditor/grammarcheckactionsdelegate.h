#ifndef GRAMMARCHECKACTIONSDELEGATE_H
#define GRAMMARCHECKACTIONSDELEGATE_H

#include <QObject>
#include "markdowntextedit.h"
#include "languagetool.h"

class GrammarCheckActionsDelegate : public QObject
{
    Q_OBJECT
public:
    explicit GrammarCheckActionsDelegate(MarkdownTextEdit* textEdit,
                                         QSharedPointer<LanguageTool> languageTool,
                                         QObject* parent = nullptr);
    virtual ~GrammarCheckActionsDelegate();

private slots:
    void grammarCheckDocument();
    void grammarCheckFinished();

private:
    MarkdownTextEdit* mTextEdit;
    QSharedPointer<LanguageTool> mLanguageTool;
    QList<QTextEdit::ExtraSelection> mGrammarCheckSelections;

    static const char* sTag;
};

#endif // GRAMMARCHECKACTIONSDELEGATE_H
