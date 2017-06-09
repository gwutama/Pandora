#include "grammarcheckactionsdelegate.h"
#include <QDebug>
#include <QScrollBar>

const char* GrammarCheckActionsDelegate::sTag = "[GrammarCheckActionsDelegate]";

GrammarCheckActionsDelegate::GrammarCheckActionsDelegate(MarkdownTextEdit* textEdit,
                                                         QSharedPointer<LanguageTool> languageTool,
                                                         QObject* parent) :
    QObject(parent),
    mTextEdit(textEdit),
    mLanguageTool(languageTool),
    mCheckOffset(0)
{
    connect(mLanguageTool.data(), &LanguageTool::suggestionsReady,
            this, &GrammarCheckActionsDelegate::checkFinished);
}


GrammarCheckActionsDelegate::~GrammarCheckActionsDelegate()
{
}


void GrammarCheckActionsDelegate::checkDocument()
{
    QString content = mTextEdit->document()->toPlainText();
    mLanguageTool->check(content);
}


void GrammarCheckActionsDelegate::checkVisibleText()
{
    if (mLanguageTool.isNull())
    {
        return;
    }

    qDebug() << sTag << "Grammar checking visible text";

    // Memorize positions
    // See: https://stackoverflow.com/questions/21955923/prevent-a-qtextedit-widget-from-scrolling-when-there-is-a-selection
    QTextCursor currentCursor = mTextEdit->textCursor();
    int scrollbarPosition = mTextEdit->verticalScrollBar()->value();

    // see: https://stackoverflow.com/questions/21493750/getting-only-the-visible-text-from-a-qtextedit-widget
    QTextCursor cursor = mTextEdit->cursorForPosition(QPoint(0, 0));
    int startPos = cursor.position();
    QPoint bottomRight(mTextEdit->viewport()->width() - 1, mTextEdit->viewport()->height() - 1);
    int endPos = mTextEdit->cursorForPosition(bottomRight).position();
    cursor.setPosition(startPos);
    cursor.setPosition(endPos, QTextCursor::KeepAnchor);
    mTextEdit->setTextCursor(cursor);

    QString content = mTextEdit->textCursor().selectedText();

    // Reset positions
    mTextEdit->setTextCursor(currentCursor);
    mTextEdit->verticalScrollBar()->setValue(scrollbarPosition);

    mCheckOffset = startPos;
    mLanguageTool->check(content);
}


void GrammarCheckActionsDelegate::checkFinished()
{
    // Memorize positions
    // See: https://stackoverflow.com/questions/21955923/prevent-a-qtextedit-widget-from-scrolling-when-there-is-a-selection
    QTextCursor currentCursor = mTextEdit->textCursor();
    int scrollbarPosition = mTextEdit->verticalScrollBar()->value();

    mSelections.clear();

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
        cur.setPosition(mCheckOffset + suggestion.offset(), QTextCursor::MoveAnchor);
        cur.setPosition(mCheckOffset + suggestion.offset() + suggestion.length(), QTextCursor::KeepAnchor);

//        qDebug() << sTag << "Selecting pos"
//                 << mCheckOffset + suggestion.offset() << "to"
//                 << mCheckOffset + suggestion.offset() + suggestion.length();

        QTextEdit::ExtraSelection extra;
        extra.cursor = cur;
        extra.format = fmt;
        mSelections.append(extra);
    }

    mTextEdit->setExtraSelections(mSelections);

    // Reset positions
    mTextEdit->setTextCursor(currentCursor);
    mTextEdit->verticalScrollBar()->setValue(scrollbarPosition);
}
