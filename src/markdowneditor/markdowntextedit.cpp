#include "markdowntextedit.h"
#include <QDebug>
#include <QScrollBar>

const char* MarkdownTextEdit::sTag = "[MarkdownTextEdit]";

MarkdownTextEdit::MarkdownTextEdit(QWidget* parent) :
    QPlainTextEdit(parent),
    mVerticalScrollPos(0)
{
    // Setup editor
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    setFont(font);
    MarkdownEditorHighlighter* mhPtr = new MarkdownEditorHighlighter(document());
    mHighlighter = QSharedPointer<MarkdownEditorHighlighter>(mhPtr);
    setContextMenuPolicy(Qt::CustomContextMenu);
    // end setup

    // Setup timers
    connect(verticalScrollBar(), &QScrollBar::valueChanged,
            &mVerticalScrollTimer, static_cast<void (QTimer::*)(void)>(&QTimer::start));
    connect(&mVerticalScrollTimer, &QTimer::timeout, this, &MarkdownTextEdit::checkVerticalScroll);
    connect(&mContentChangeTimer, &QTimer::timeout, this, &MarkdownTextEdit::checkTextChanged);

    mVerticalScrollTimer.setInterval(1000);
    mVerticalScrollTimer.setSingleShot(true);
    mContentChangeTimer.setInterval(7000);
    mContentChangeTimer.setSingleShot(false);

    mContentChangeTimer.start();
}


MarkdownTextEdit::~MarkdownTextEdit()
{
}


void MarkdownTextEdit::increaseFontSize()
{
    QFont newFont = font();
    newFont.setPointSize(newFont.pointSize() + 1);
    setFont(newFont);
}


void MarkdownTextEdit::decreaseFontSize()
{
    QFont newFont = font();
    newFont.setPointSize(newFont.pointSize() - 1);
    setFont(newFont);
}


void MarkdownTextEdit::setMargin(unsigned int size)
{
    document()->setDocumentMargin(size);
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
