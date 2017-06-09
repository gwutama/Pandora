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

    inline QList<QTextEdit::ExtraSelection> selections()
    { return mSelections; }

public slots:
    void checkVisibleText();

private slots:
    void checkDocument();
    void checkFinished();

private:
    MarkdownTextEdit* mTextEdit;
    QSharedPointer<LanguageTool> mLanguageTool;
    QList<QTextEdit::ExtraSelection> mSelections;
    int mCheckOffset;

    static const char* sTag;
};

#endif // GRAMMARCHECKACTIONSDELEGATE_H
