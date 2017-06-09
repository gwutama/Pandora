#include "markdowntextedit.h"
#include <QDebug>
#include <QMenu>
#include <QScrollBar>

const char* MarkdownTextEdit::sTag = "[MarkdownTextEdit]";

MarkdownTextEdit::MarkdownTextEdit(QWidget* parent) :
    QPlainTextEdit(parent),
    mVerticalScrollPos(0)
{
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(verticalScrollBar(), &QScrollBar::valueChanged,
            &mVerticalScrollTimer, static_cast<void (QTimer::*)(void)>(&QTimer::start));

    mVerticalScrollTimer.setInterval(1000);
    mVerticalScrollTimer.setSingleShot(true);

    mContentChangeTimer.setInterval(7000);
    mContentChangeTimer.setSingleShot(false);

    connect(&mVerticalScrollTimer, &QTimer::timeout, this, &MarkdownTextEdit::checkVerticalScroll);
    connect(&mContentChangeTimer, &QTimer::timeout, this, &MarkdownTextEdit::checkTextChanged);

    mContentChangeTimer.start();
}


MarkdownTextEdit::~MarkdownTextEdit()
{
}


void MarkdownTextEdit::showContextMenu(const QStringList& suggestions,
                                       const QPoint& point)
{
    mSuggestionActions.clear();

    QMenu* menu = createStandardContextMenu();
    menu->addSeparator();

    for (int i = 0; i < suggestions.size(); i++)
    {
        QAction* action = menu->addAction(suggestions.at(i));
        action->setData(suggestions.at(i));

        connect(action, &QAction::triggered, this, &MarkdownTextEdit::onSuggestionActionTriggered);

        mSuggestionActions.append(action);
    }

    menu->exec(mapToGlobal(point));
    delete menu;
}


void MarkdownTextEdit::onSuggestionActionTriggered()
{
    QObject* obj = sender();
    QAction* action = qobject_cast<QAction*>(obj);
    QString replacement = action->data().toString();
    emit suggestionActionTriggered(replacement);
}


void MarkdownTextEdit::checkVerticalScroll()
{
    int pos = verticalScrollBar()->value();

    if (abs(pos - mVerticalScrollPos) > 10)
    {
        mVerticalScrollPos = pos;
        emit laxVerticalScrollEnd(pos);
    }
}


void MarkdownTextEdit::checkTextChanged()
{
    QString content = document()->toPlainText();

    if (mOldContent != content)
    {
        mOldContent = content;
        emit laxTextChanged(content);
    }
}


void MarkdownTextEdit::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Tab)
    {
        indent(event);
        return;
    }
    else if (event->key() == Qt::Key_Backtab)
    {
        outdent(event);
        return;
    }

    QPlainTextEdit::keyPressEvent(event);
}


void MarkdownTextEdit::indent(QKeyEvent* event)
{
    QTextCursor currentCursor = textCursor();
    QString currentText = currentCursor.selectedText();

    if (currentText.isEmpty())
    {
        currentCursor.insertText("  " + currentText);
        event->accept();
        return;
    }
    else
    {
        QStringList lines = currentText.split(QChar::ParagraphSeparator);
        QString newText;

        foreach (QString line, lines)
        {
            newText += "  " + line + "\n";
        }

        newText.chop(1);
        currentCursor.insertText(newText);
        event->accept();
        return;
    }
}


void MarkdownTextEdit::outdent(QKeyEvent* event)
{
    QTextCursor currentCursor = textCursor();
    QString currentText = currentCursor.selectedText();

    if (currentText.isEmpty())
    {
        for (int i = 0; i < 2; i++)
        {
            currentCursor.movePosition(QTextCursor::Left);
            QChar c = document()->characterAt(currentCursor.position());

            if (c == ' ')
            {
                currentCursor.deleteChar();
            }
            else
            {
                break;
            }
        }

        event->accept();
        return;
    }
    else
    {
        QStringList lines = currentText.split(QChar::ParagraphSeparator);
        QString newText;

        foreach (QString line, lines)
        {
            for (int i = 0; i < 2; i++)
            {
                if (line.startsWith(' '))
                {
                    line.remove(0, 1);
                }
                else
                {
                    break;
                }
            }

            newText += line + "\n";
        }

        newText.chop(1);
        currentCursor.insertText(newText);
        event->accept();
        return;
    }
}


void MarkdownTextEdit::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        // move cursor to the nearest position on right click
        QTextCursor nearestCursor = cursorForPosition(event->pos());
        setTextCursor(nearestCursor);
        qDebug() << sTag << "Nearest cursor position to mouse pointer:" << nearestCursor.position();
    }

    QPlainTextEdit::mousePressEvent(event);
}
