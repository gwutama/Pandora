#include "grammarcheckactionsdelegate.h"
#include <QDebug>
#include <QScrollBar>
#include <QAction>

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
    mMatches.clear();

    QTextCharFormat fmt;
    fmt.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    fmt.setUnderlineColor(Qt::blue);

    mMatches = mLanguageTool->suggestions();
    qDebug() << sTag << "Highlighting suggestions:" << mMatches.size();

    for (int i = 0; i < mMatches.size(); i++)
    {
        LanguageToolMatch suggestion = mMatches.at(i);

        // Select text
        // From https://stackoverflow.com/questions/21122928/selecting-a-piece-of-text-using-qtextcursor
        QTextCursor cur = currentCursor;
        int start = mCheckOffset + suggestion.offset();
        int end = mCheckOffset + suggestion.offset() + suggestion.length();
        cur.setPosition(start, QTextCursor::MoveAnchor);
        cur.setPosition(end, QTextCursor::KeepAnchor);

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
    foreach (LanguageToolMatch match, mMatches)
    {
        int start = mCheckOffset + match.offset();
        int end = mCheckOffset + match.offset() + match.length();

        if (cursor.position() >= start && cursor.position() <= end)
        {
            suggestions = match.replacements();
            qDebug() << sTag << "Replacements:" << match.replacements();
            mSelectedMatch = match;
            break;
        }
    }

    showContextMenuWithSuggestions(point, suggestions);
}


void GrammarCheckActionsDelegate::replaceSelectionWithTextReplacement()
{
    QObject* obj = sender();
    QAction* action = qobject_cast<QAction*>(obj);
    QString replacement = action->data().toString();

    // Select text to be replaced
    QTextCursor cur = mTextEdit->textCursor();
    int start = mCheckOffset + mSelectedMatch.offset();
    int end = mCheckOffset + mSelectedMatch.offset() + mSelectedMatch.length();
    cur.setPosition(start, QTextCursor::MoveAnchor);
    cur.setPosition(end, QTextCursor::KeepAnchor);

    // Replace selection
    cur.insertText(replacement);

    checkVisibleText();
}

