#include "grammarcheckactionsdelegate.h"
#include <QDebug>
#include <QScrollBar>

const char* GrammarCheckActionsDelegate::sTag = "[GrammarCheckActionsDelegate]";

GrammarCheckActionsDelegate::GrammarCheckActionsDelegate(MarkdownTextEdit* textEdit,
                                                         QSharedPointer<LanguageTool> languageTool,
                                                         QObject* parent) :
    ProofreadActionsDelegateBase(textEdit, parent),
    mLanguageTool(languageTool),
    mCheckOffset(0)
{
    connect(mLanguageTool.data(), &LanguageTool::suggestionsReady,
            this, &GrammarCheckActionsDelegate::checkFinished);
}


GrammarCheckActionsDelegate::~GrammarCheckActionsDelegate()
{
}


void GrammarCheckActionsDelegate::runDocumentCheck(const QString& text)
{
    if (mLanguageTool.isNull())
    {
        return;
    }

    qDebug() << sTag << "Grammar checking document";
    mLanguageTool->check(text);
}


void GrammarCheckActionsDelegate::runVisibleTextCheck(const QString& text, int startPos, int endPos)
{
    if (mLanguageTool.isNull())
    {
        return;
    }

    qDebug() << sTag << "Grammar checking visible text";
    mCheckOffset = startPos;
    mLanguageTool->check(text);
}


void GrammarCheckActionsDelegate::checkFinished()
{
    // Memorize positions
    // See: https://stackoverflow.com/questions/21955923/prevent-a-qtextedit-widget-from-scrolling-when-there-is-a-selection
    QTextCursor currentCursor = mTextEdit->textCursor();
    int scrollbarPosition = mTextEdit->verticalScrollBar()->value();

    mSelections.clear();
    mSuggestions.clear();

    QTextCharFormat fmt;
    fmt.setUnderlineStyle(QTextCharFormat::DashDotDotLine);
    fmt.setUnderlineColor(Qt::blue);

    mSuggestions = mLanguageTool->suggestions();
    qDebug() << sTag << "Highlighting suggestions:" << mSuggestions.size();

    for (int i = 0; i < mSuggestions.size(); i++)
    {
        LanguageToolMatch suggestion = mSuggestions.at(i);

        // Select text
        // From https://stackoverflow.com/questions/21122928/selecting-a-piece-of-text-using-qtextcursor
        QTextCursor cur = currentCursor;
        cur.setPosition(mCheckOffset + suggestion.offset(), QTextCursor::MoveAnchor);
        cur.setPosition(mCheckOffset + suggestion.offset() + suggestion.length(), QTextCursor::KeepAnchor);

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


void GrammarCheckActionsDelegate::showContextMenu(const QPoint& point)
{
    QTextCursor cursor = mTextEdit->textCursor();
    QStringList suggestions;

    // Are we on one of the possibly incorrect words?
    foreach (LanguageToolMatch match, mSuggestions)
    {
        if (cursor.position() >= mCheckOffset + match.offset() &&
                cursor.position() <= mCheckOffset + match.offset() + match.length())
        {
            suggestions = match.replacements();
            qDebug() << sTag << "Replacements:" << match.replacements();
            break;
        }
    }

    showContextMenuWithSuggestions(point, suggestions);
}
