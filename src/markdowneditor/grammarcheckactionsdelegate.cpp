#include "grammarcheckactionsdelegate.h"
#include <QDebug>

const char* GrammarCheckActionsDelegate::sTag = "[GrammarCheckActionsDelegate]";

GrammarCheckActionsDelegate::GrammarCheckActionsDelegate(MarkdownTextEdit* textEdit,
                                                         QSharedPointer<LanguageTool> languageTool,
                                                         QObject* parent) :
    QObject(parent),
    mTextEdit(textEdit),
    mLanguageTool(languageTool)
{
    connect(mLanguageTool.data(), &LanguageTool::suggestionsReady,
            this, &GrammarCheckActionsDelegate::grammarCheckFinished);
}


GrammarCheckActionsDelegate::~GrammarCheckActionsDelegate()
{
}


void GrammarCheckActionsDelegate::grammarCheckDocument()
{
    QString content = mTextEdit->document()->toPlainText();
    mLanguageTool->check(content);
}


void GrammarCheckActionsDelegate::grammarCheckFinished()
{
    mGrammarCheckSelections.clear();
    QTextCursor currentCursor = mTextEdit->textCursor();
    mTextEdit->moveCursor(QTextCursor::Start);

    QTextCharFormat fmt;
    fmt.setUnderlineStyle(QTextCharFormat::DashDotDotLine);
    fmt.setUnderlineColor(Qt::blue);

    QList<LanguageToolMatch> suggestions = mLanguageTool->suggestions();

    qDebug() << sTag << "Highlighting suggestions:" << suggestions.size();

    for (int i = 0; i < suggestions.size(); i++)
    {
        LanguageToolMatch suggestion = suggestions.at(i);

        // Select text
        // From https://stackoverflow.com/questions/21122928/selecting-a-piece-of-text-using-qtextcursor
        QTextCursor cur = currentCursor;
        cur.setPosition(suggestion.offset(), QTextCursor::MoveAnchor);
        cur.setPosition(suggestion.offset() + suggestion.length(), QTextCursor::KeepAnchor);

        qDebug() << sTag << "Selecting pos" << suggestion.offset() << "to" << suggestion.offset() +
                 suggestion.length();

        QTextEdit::ExtraSelection extra;
        extra.cursor = cur;
        extra.format = fmt;
        mGrammarCheckSelections.append(extra);
    }

    mTextEdit->setExtraSelections(mGrammarCheckSelections);
    mTextEdit->setTextCursor(currentCursor);
}
