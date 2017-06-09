#include "markdowntextedit.h"
#include <QDebug>
#include <QMenu>
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

    // Setup spell checker
    connect(this, &MarkdownTextEdit::customContextMenuRequested,
            this, &MarkdownTextEdit::showContextMenu);
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


void MarkdownTextEdit::onSuggestionActionTriggered()
{
    QObject* obj = sender();
    QAction* action = qobject_cast<QAction*>(obj);
    QString replacement = action->data().toString();
    replaceSelection(replacement);
}


void MarkdownTextEdit::checkVerticalScroll()
{
    int pos = verticalScrollBar()->value();

    if (abs(pos - mVerticalScrollPos) > 10)
    {
        mVerticalScrollPos = pos;
        emit laxVerticalScrollEnd(pos);

        spellcheckVisibleText();
    }
}


void MarkdownTextEdit::checkTextChanged()
{
    QString content = document()->toPlainText();

    if (mOldContent != content)
    {
        mOldContent = content;
        emit laxTextChanged(content);

        spellcheckVisibleText();
    }
}


void MarkdownTextEdit::replaceSelection(const QString& replacement)
{
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    cursor.insertText(replacement);
}


void MarkdownTextEdit::spellcheckVisibleText()
{
    if (mSpellCheck.isNull())
    {
        return;
    }

    qDebug() << sTag << "Spell checking visible text";

    // Memorize positions
    // See: https://stackoverflow.com/questions/21955923/prevent-a-qtextedit-widget-from-scrolling-when-there-is-a-selection
    QTextCursor currentCursor = textCursor();
    int scrollbarPosition = verticalScrollBar()->value();

    // see: https://stackoverflow.com/questions/21493750/getting-only-the-visible-text-from-a-qtextedit-widget
    QTextCursor cursor = cursorForPosition(QPoint(0, 0));
    QPoint bottomRight(viewport()->width() - 1, viewport()->height() - 1);
    int endPos = cursorForPosition(bottomRight).position();
    setTextCursor(cursor);

    // actual spell checking
    mSpellCheckSelections.clear();

    QTextCharFormat fmt;
    fmt.setUnderlineStyle(QTextCharFormat::DashDotDotLine);
    fmt.setUnderlineColor(Qt::red);

    while (find(QRegExp("((?!_)\\w)+")))
    {
        QString word = textCursor().selectedText();

        if (!mSpellCheck->spellcheck(word))
        {
            QTextEdit::ExtraSelection extra;
            extra.cursor = textCursor();
            extra.format = fmt;
            mSpellCheckSelections.append(extra);
        }

        if (textCursor().position() > endPos)
        {
            break;
        }
    }

    setExtraSelections(mSpellCheckSelections);

    // Set positions back to previous ones
    setTextCursor(currentCursor);
    verticalScrollBar()->setValue(scrollbarPosition);
}


void MarkdownTextEdit::showContextMenu(const QPoint& point)
{
    if (mSpellCheck.isNull())
    {
        showContextMenuWithSuggestions(point);
        return;
    }

    // Get word under cursor
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    QString word = cursor.selectedText();

    // Are we on one of the possibly incorrect words?
    bool showSuggestions = false;

    foreach (QTextEdit::ExtraSelection selection, mSpellCheckSelections)
    {
        if (word == selection.cursor.selectedText())
        {
            showSuggestions = true;
            break;
        }
    }

    // Retrieve suggestions and show context menu with suggestions
    QStringList suggestions;

    if (showSuggestions)
    {
        suggestions = mSpellCheck->suggest(word);
    }

    showContextMenuWithSuggestions(point, suggestions);
}


void MarkdownTextEdit::showContextMenuWithSuggestions(const QPoint& point,
                                                      const QStringList& suggestions)
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
