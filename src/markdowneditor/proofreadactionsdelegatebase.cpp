#include "proofreadactionsdelegatebase.h"
#include <QDebug>
#include <QScrollBar>
#include <QMenu>

const char* ProofreadActionsDelegateBase::sTag = "[ProofreadActionsDelegateBase]";

ProofreadActionsDelegateBase::ProofreadActionsDelegateBase(MarkdownTextEdit* textEdit,
                                                           QObject* parent) :
    QObject(parent),
    mTextEdit(textEdit)
{
    connect(mTextEdit, &MarkdownTextEdit::customContextMenuRequested,
            this, &ProofreadActionsDelegateBase::showContextMenu,
            Qt::UniqueConnection);
}


ProofreadActionsDelegateBase::~ProofreadActionsDelegateBase()
{
}


void ProofreadActionsDelegateBase::checkVisibleText()
{
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
    runVisibleTextCheck(content, startPos, endPos);

    // Set positions back to previous ones
    mTextEdit->setTextCursor(currentCursor);
    mTextEdit->verticalScrollBar()->setValue(scrollbarPosition);
}


void ProofreadActionsDelegateBase::checkDocument()
{
    // Memorize positions
    // See: https://stackoverflow.com/questions/21955923/prevent-a-qtextedit-widget-from-scrolling-when-there-is-a-selection
    QTextCursor currentCursor = mTextEdit->textCursor();
    int scrollbarPosition = mTextEdit->verticalScrollBar()->value();

    QString content = mTextEdit->document()->toPlainText();
    runDocumentCheck(content);

    // Set positions back to previous ones
    mTextEdit->setTextCursor(currentCursor);
    mTextEdit->verticalScrollBar()->setValue(scrollbarPosition);
}


void ProofreadActionsDelegateBase::showContextMenuWithSuggestions(const QPoint& point,
                                                                  const QStringList& suggestions)
{
    mSuggestionActions.clear();

    QMenu* menu = mTextEdit->createStandardContextMenu();
    menu->addSeparator();

    for (int i = 0; i < suggestions.size(); i++)
    {
        QAction* action = menu->addAction(suggestions.at(i));
        action->setData(suggestions.at(i));

        connect(action, &QAction::triggered,
                this, &ProofreadActionsDelegateBase::replaceSelectionWithTextReplacement);

        mSuggestionActions.append(action);
    }

    menu->exec(mTextEdit->mapToGlobal(point));
    delete menu;
}
