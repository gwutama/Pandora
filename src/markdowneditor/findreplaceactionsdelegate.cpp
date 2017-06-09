#include "findreplaceactionsdelegate.h"
#include <QDebug>

const char* FindReplaceActionsDelegate::sTag = "[FindReplaceActionsDelegate]";

FindReplaceActionsDelegate::FindReplaceActionsDelegate(MarkdownTextEdit* textEdit,
                                                       FindReplaceWidget* findReplaceWidget,
                                                       QObject* parent) :
    QObject(parent),
    mTextEdit(textEdit),
    mFindReplaceWidget(findReplaceWidget)
{
    // Signals slots with Find/replace widget
    connect(mFindReplaceWidget, &FindReplaceWidget::wantToExecuteSearch,
            this, &FindReplaceActionsDelegate::searchAndHighlightMatches);
    connect(mFindReplaceWidget, &FindReplaceWidget::searchTermChanged,
            this, &FindReplaceActionsDelegate::searchAndHighlightMatches);
    connect(mFindReplaceWidget, &FindReplaceWidget::nextButtonClicked,
            this, &FindReplaceActionsDelegate::goToNextMatch);
    connect(mFindReplaceWidget, &FindReplaceWidget::previousButtonClicked,
            this, &FindReplaceActionsDelegate::goToPreviousMatch);
    connect(mFindReplaceWidget, &FindReplaceWidget::replaceButtonClicked,
            this, &FindReplaceActionsDelegate::replaceMatch);
    connect(mFindReplaceWidget, &FindReplaceWidget::replaceFindButtonClicked,
            this, &FindReplaceActionsDelegate::replaceMatchGoToNextOne);
    connect(mFindReplaceWidget, &FindReplaceWidget::replaceAllButtonClicked,
            this, &FindReplaceActionsDelegate::replaceMatches);
}


FindReplaceActionsDelegate::~FindReplaceActionsDelegate()
{
}


void FindReplaceActionsDelegate::removeHighlightMatches()
{
    mMatchTextSelections.clear();
    mTextEdit->setExtraSelections(mMatchTextSelections);
}


void FindReplaceActionsDelegate::searchAndHighlightMatches()
{
    QString searchString = mFindReplaceWidget->searchTerm();

    if (searchString.isNull())
    {
        return;
    }

    // https://stackoverflow.com/questions/7624196/find-all-strings-wanted-and-select-them-with-qplaintexteditsetextraselections
    qDebug() << sTag << "Searching string in document:" << searchString;

    mMatchTextSelections.clear();
    QTextCursor currentCursor = mTextEdit->textCursor();
    mTextEdit->moveCursor(QTextCursor::Start);

    QTextCharFormat colorFormat;
    colorFormat.setBackground(Qt::yellow);

    while (mTextEdit->find(searchString))
    {
        QTextEdit::ExtraSelection extra;
        extra.cursor = mTextEdit->textCursor();
        extra.format = colorFormat;
        mMatchTextSelections.append(extra);
    }

    mTextEdit->setExtraSelections(mMatchTextSelections);
    mFindReplaceWidget->setNumMatches(mMatchTextSelections.size());

    // go directly to the next nearest match
    goToNearestMatch(currentCursor);
}


bool FindReplaceActionsDelegate::goToNextMatch()
{
    if (!mMatchTextSelections.size())
    {
        qWarning() << sTag << "No matches found. Not going to next match";
        return false;
    }

    QTextCursor currentCursor = mTextEdit->textCursor();

    foreach (QTextEdit::ExtraSelection selection, mMatchTextSelections)
    {
        QTextCursor selectionCursor = selection.cursor;

        if (selectionCursor.position() > currentCursor.position())
        {
            mTextEdit->setTextCursor(selectionCursor);
            return true;
        }
    }

    // Cursor hasn't been set. cycle to first selection in the list.
    return goToNthMatch(0);
}


bool FindReplaceActionsDelegate::goToPreviousMatch()
{
    if (!mMatchTextSelections.size())
    {
        qWarning() << sTag << "No matches found. Not going to previous match";
        return false;
    }

    QTextCursor currentCursor = mTextEdit->textCursor();
    QList<QTextEdit::ExtraSelection>::iterator iter;

    for (iter = mMatchTextSelections.end() - 1; iter != mMatchTextSelections.begin() - 1; --iter)
    {
        QTextCursor selectionCursor = iter->cursor;

        if (selectionCursor.position() < currentCursor.position())
        {
            mTextEdit->setTextCursor(selectionCursor);
            return true;
        }
    }

    // Cursor hasn't been set. cycle to last selection in the list.
    return goToNthMatch(mMatchTextSelections.size() - 1);
}


bool FindReplaceActionsDelegate::goToNthMatch(unsigned int index)
{
    if (mMatchTextSelections.size() <= index)
    {
        qWarning() << sTag << "No matches found. Not going to nth match";
        return false;
    }

    QTextCursor firstPos = mMatchTextSelections.at(index).cursor;
    mTextEdit->setTextCursor(firstPos);
    return true;
}


bool FindReplaceActionsDelegate::goToNearestMatch(const QTextCursor& cursor)
{
    if (!mMatchTextSelections.size()) // ignore if we havent searched
    {
        qWarning() << sTag << "No matches found. Not going to replace match";
        return false;
    }

    int delta = mTextEdit->toPlainText().size();
    QTextCursor nearestMatchCursor;

    foreach (QTextEdit::ExtraSelection selection, mMatchTextSelections)
    {
        QTextCursor selectionCursor = selection.cursor;
        int tmpDelta = abs(selectionCursor.position() - cursor.position());

        if (tmpDelta < delta)
        {
            delta = tmpDelta;
            nearestMatchCursor = selectionCursor;
        }
    }

    mTextEdit->setTextCursor(nearestMatchCursor);
    return true;
}


bool FindReplaceActionsDelegate::replaceMatch()
{
    if (!mMatchTextSelections.size()) // ignore if we havent searched
    {
        qWarning() << sTag << "No matches found. Not going to replace match";
        return false;
    }

    // if the cursor is not at a match, go to the next match
    QTextCursor currentCursor = mTextEdit->textCursor();
    QTextCursor cursorToReplace;
    int indexToReplace = -1;

    for (int i = 0; i < mMatchTextSelections.size(); i++)
    {
        QTextEdit::ExtraSelection selection = mMatchTextSelections.at(i);
        QTextCursor selectionCursor = selection.cursor;

        if (selectionCursor.position() == currentCursor.position())
        {
            cursorToReplace = selectionCursor;
            indexToReplace = i;
            break;
        }
    }

    if (cursorToReplace.isNull()) // we didnt found a match
    {
        return false;
    }

    QString replacementString = mFindReplaceWidget->replacementText();
    cursorToReplace.insertText(replacementString);
    mMatchTextSelections.removeAt(indexToReplace);

    // Reduce the number of matches text in the find widget
    mFindReplaceWidget->setNumMatches(mFindReplaceWidget->numMatches() - 1);
    return true;
}


bool FindReplaceActionsDelegate::replaceMatchGoToNextOne()
{
    if (replaceMatch())
    {
        return goToNextMatch();
    }

    qWarning() << sTag << "Cannot replace match";
    return false;
}


bool FindReplaceActionsDelegate::replaceMatches()
{
    if (!mMatchTextSelections.size()) // ignore if we havent searched
    {
        qWarning() << sTag << "No matches found. Not going to replace all matches";
        return false;
    }

    QString replacementString = mFindReplaceWidget->replacementText();

    foreach (QTextEdit::ExtraSelection selection, mMatchTextSelections)
    {
        QTextCursor selectionCursor = selection.cursor;
        selectionCursor.insertText(replacementString);
    }

    // Clear all selections and set num matches to zero
    mMatchTextSelections.clear();
    mFindReplaceWidget->setNumMatches(0);
    return true;
}
