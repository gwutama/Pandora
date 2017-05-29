#include "markdowneditor.h"
#include "ui_markdowneditor.h"
#include <QTextEdit>
#include <QFileInfo>
#include <QDebug>
#include <QShortcut>
#include "insertmodifylinkdialog.h"

const char* MarkdownEditor::sTag = "[MarkdownEditor]";

MarkdownEditor::MarkdownEditor(QSharedPointer<AppConfig> config,
                               QWidget* parent) :
    QWidget(parent),
    mConfig(config),
    mUi(new Ui::MarkdownEditor)
{
    mUi->setupUi(this);
    setupEditor();

    mUi->findReplaceWidget->hide();

    // Find/replace widget
    connect(mUi->findReplaceWidget, &FindReplaceWidget::wantToExecuteSearch,
            this, &MarkdownEditor::searchAndHighlightMatches);
    connect(mUi->findReplaceWidget, &FindReplaceWidget::searchTermChanged,
            this, &MarkdownEditor::searchAndHighlightMatches);
    connect(mUi->findReplaceWidget, &FindReplaceWidget::nextButtonClicked,
            this, &MarkdownEditor::goToNextMatch);
    connect(mUi->findReplaceWidget, &FindReplaceWidget::previousButtonClicked,
            this, &MarkdownEditor::goToPreviousMatch);
    connect(mUi->findReplaceWidget, &FindReplaceWidget::replaceButtonClicked,
            this, &MarkdownEditor::replaceMatch);
    connect(mUi->findReplaceWidget, &FindReplaceWidget::replaceFindButtonClicked,
            this, &MarkdownEditor::replaceMatchGoToNextOne);
    connect(mUi->findReplaceWidget, &FindReplaceWidget::replaceAllButtonClicked,
            this, &MarkdownEditor::replaceMatches);

    connect(&mContentChangeTimer, &QTimer::timeout, this, &MarkdownEditor::checkContentChanged);

    QShortcut* escKeyShortcut = new QShortcut(Qt::Key_Escape, parent);
    connect(escKeyShortcut, &QShortcut::activated, this, &MarkdownEditor::onEscKeyActivated);
}


MarkdownEditor::~MarkdownEditor()
{
    delete mUi;
}


QString MarkdownEditor::content()
{
    return mUi->textEdit->document()->toPlainText();
}


void MarkdownEditor::setMargin(unsigned int size)
{
    QTextDocument* doc = mUi->textEdit->document();
    doc->setDocumentMargin(size);
}


void MarkdownEditor::onEscKeyActivated()
{
    if (!isEnabled())
    {
        return;
    }

    if (!mUi->findReplaceWidget->isHidden())
    {
        mUi->findReplaceWidget->hide();
        removeHighlightMatches();
    }
}

void MarkdownEditor::checkContentChanged()
{
    QString content = mUi->textEdit->document()->toPlainText();

    if (mOldContent != content)
    {
        emit contentChanged(content);
        mOldContent = content;
    }
}


void MarkdownEditor::refocusEditor()
{
    mUi->textEdit->setFocus();
}

bool MarkdownEditor::open()
{
    if (openFile(mConfig->markdownFile()))
    {
        qDebug() << sTag << "Content has been changed";
        mContentChangeTimer.setSingleShot(false);
        mContentChangeTimer.start(3000); // 3 seconds
        checkContentChanged();
        return true;
    }

    return false;
}


void MarkdownEditor::close()
{
    mUi->textEdit->clear();
    mContentChangeTimer.stop();
}


bool MarkdownEditor::save()
{
    return saveAs(mConfig->markdownFile());
}


bool MarkdownEditor::saveAs(const QString& path)
{
    if (path.isEmpty())
    {
        qWarning() << sTag << "Cannot save because file path is empty";
        return false;
    }

    QFile file(path);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        qWarning() << sTag << "Cannot open file for writing:" << path;
        return false;
    }

    QString content = mUi->textEdit->document()->toPlainText();
    qDebug() << sTag << "Saving content to file:" << content;

    file.write(content.toUtf8());
    file.close();
    return true;
}


bool MarkdownEditor::openFile(const QString& path)
{
    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qWarning() << sTag << "Cannot open file for reading:" << path;
        return false;
    }

    QByteArray content = file.readAll();
    QTextDocument* doc = mUi->textEdit->document();
    doc->setPlainText(content);

    file.close();
    return true;
}


void MarkdownEditor::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(mConfig->editorFontPointSize());
    mUi->textEdit->setFont(font);

    QFontMetrics metrics(font);
    mUi->textEdit->setTabStopWidth(4 * metrics.width(' ')); // replace tab with 4 spaces

    MarkdownEditorHighlighter* mhPtr = new MarkdownEditorHighlighter(mUi->textEdit->document());
    mHighlighter = QSharedPointer<MarkdownEditorHighlighter>(mhPtr);

    setNarrowMargin(); // default margin
}


void MarkdownEditor::increaseFontSize()
{
    QFont font = mUi->textEdit->font();
    font.setPointSize(font.pointSize() + 1);
    mUi->textEdit->setFont(font);

    // Save
    mConfig->setEditorFontPointSize(mUi->textEdit->font().pointSize());
    mConfig->save();
}


void MarkdownEditor::decreaseFontSize()
{
    QFont font = mUi->textEdit->font();
    font.setPointSize(font.pointSize() - 1);
    mUi->textEdit->setFont(font);

    // Save
    mConfig->setEditorFontPointSize(mUi->textEdit->font().pointSize());
    mConfig->save();
}


void MarkdownEditor::showFindReplaceWidget()
{
    if (isEnabled())
    {
        mUi->findReplaceWidget->show();
        mUi->findReplaceWidget->setFocus();
    }
}


void MarkdownEditor::searchAndHighlightMatches()
{
    QString searchString = mUi->findReplaceWidget->searchTerm();

    if (searchString.isNull())
    {
        return;
    }

    // https://stackoverflow.com/questions/7624196/find-all-strings-wanted-and-select-them-with-qplaintexteditsetextraselections
    qDebug() << sTag << "Searching string in document:" << searchString;

    mMatchTextSelections.clear();
    QTextCursor currentCursor = mUi->textEdit->textCursor();
    mUi->textEdit->moveCursor(QTextCursor::Start);

    QTextCharFormat colorFormat;
    colorFormat.setBackground(Qt::yellow);

    while (mUi->textEdit->find(searchString))
    {
        QTextEdit::ExtraSelection extra;
        extra.cursor = mUi->textEdit->textCursor();
        extra.format = colorFormat;
        mMatchTextSelections.append(extra);
    }

    mUi->textEdit->setExtraSelections(mMatchTextSelections);
    mUi->findReplaceWidget->setNumMatches(mMatchTextSelections.size());

    // go directly to the next nearest match
    goToNearestMatch(currentCursor);
}


void MarkdownEditor::removeHighlightMatches()
{
    mMatchTextSelections.clear();
    mUi->textEdit->setExtraSelections(mMatchTextSelections);
}


bool MarkdownEditor::goToNextMatch()
{
    if (!mMatchTextSelections.size())
    {
        qWarning() << sTag << "No matches found. Not going to next match";
        return false;
    }

    QTextCursor currentCursor = mUi->textEdit->textCursor();

    foreach (QTextEdit::ExtraSelection selection, mMatchTextSelections)
    {
        QTextCursor selectionCursor = selection.cursor;

        if (selectionCursor.position() > currentCursor.position())
        {
            mUi->textEdit->setTextCursor(selectionCursor);
            return true;
        }
    }

    // Cursor hasn't been set. cycle to first selection in the list.
    return goToNthMatch(0);
}


bool MarkdownEditor::goToPreviousMatch()
{
    if (!mMatchTextSelections.size())
    {
        qWarning() << sTag << "No matches found. Not going to previous match";
        return false;
    }

    QTextCursor currentCursor = mUi->textEdit->textCursor();
    QList<QTextEdit::ExtraSelection>::iterator iter;

    for (iter = mMatchTextSelections.end() - 1; iter != mMatchTextSelections.begin() - 1; --iter)
    {
        QTextCursor selectionCursor = iter->cursor;

        if (selectionCursor.position() < currentCursor.position())
        {
            mUi->textEdit->setTextCursor(selectionCursor);
            return true;
        }
    }

    // Cursor hasn't been set. cycle to last selection in the list.
    return goToNthMatch(mMatchTextSelections.size() - 1);
}


bool MarkdownEditor::goToNthMatch(unsigned int index)
{
    if (mMatchTextSelections.size() <= index)
    {
        qWarning() << sTag << "No matches found. Not going to nth match";
        return false;
    }

    QTextCursor firstPos = mMatchTextSelections.at(index).cursor;
    mUi->textEdit->setTextCursor(firstPos);
    return true;
}


bool MarkdownEditor::goToNearestMatch(const QTextCursor& cursor)
{
    if (!mMatchTextSelections.size()) // ignore if we havent searched
    {
        qWarning() << sTag << "No matches found. Not going to replace match";
        return false;
    }

    int delta = mUi->textEdit->toPlainText().size();
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

    mUi->textEdit->setTextCursor(nearestMatchCursor);
    return true;
}


bool MarkdownEditor::replaceMatch()
{
    if (!mMatchTextSelections.size()) // ignore if we havent searched
    {
        qWarning() << sTag << "No matches found. Not going to replace match";
        return false;
    }

    // if the cursor is not at a match, go to the next match
    QTextCursor currentCursor = mUi->textEdit->textCursor();
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

    QString replacementString = mUi->findReplaceWidget->replacementText();
    cursorToReplace.insertText(replacementString);
    mMatchTextSelections.removeAt(indexToReplace);

    // Reduce the number of matches text in the find widget
    mUi->findReplaceWidget->setNumMatches(mUi->findReplaceWidget->numMatches() - 1);
    return true;
}


bool MarkdownEditor::replaceMatchGoToNextOne()
{
    if (replaceMatch())
    {
        return goToNextMatch();
    }

    qWarning() << sTag << "Cannot replace match";
    return false;
}


bool MarkdownEditor::replaceMatches()
{
    if (!mMatchTextSelections.size()) // ignore if we havent searched
    {
        qWarning() << sTag << "No matches found. Not going to replace all matches";
        return false;
    }

    QString replacementString = mUi->findReplaceWidget->replacementText();

    foreach (QTextEdit::ExtraSelection selection, mMatchTextSelections)
    {
        QTextCursor selectionCursor = selection.cursor;
        selectionCursor.insertText(replacementString);
    }

    // Clear all selections and set num matches to zero
    mMatchTextSelections.clear();
    mUi->findReplaceWidget->setNumMatches(0);
    return true;
}


void MarkdownEditor::toggleSelectionBold()
{
    QTextCursor currentCursor = mUi->textEdit->textCursor();
    QString text = currentCursor.selectedText();

    if (text.isEmpty())
    {
        return;
    }

    QString newText;

    QRegExp boldAsteriskRx("\\*{2}.*\\*{2}");
    QRegExp boldUnderlineRx("_{2}.*_{2}");

    if (text.indexOf(boldAsteriskRx) > -1 || text.indexOf(boldUnderlineRx) > -1)
    {
        newText = text.mid(2, text.size() - 4);
    }
    else
    {
        newText = "**" + text + "**";
    }

    currentCursor.insertText(newText);
}


void MarkdownEditor::toggleSelectionItalic()
{
    QTextCursor currentCursor = mUi->textEdit->textCursor();
    QString text = currentCursor.selectedText();

    if (text.isEmpty())
    {
        return;
    }

    QString newText;

    QRegExp italicAsteriskRx("\\*.*\\*");
    QRegExp italicUnderlineRx("_.*_");

    if (text.indexOf(italicAsteriskRx) > -1 || text.indexOf(italicUnderlineRx) > -1)
    {
        newText = text.mid(1, text.size() - 2);
    }
    else
    {
        newText = "*" + text + "*";
    }

    currentCursor.insertText(newText);
}


void MarkdownEditor::toggleSelectionStrikeout()
{
    QTextCursor currentCursor = mUi->textEdit->textCursor();
    QString text = currentCursor.selectedText();

    if (text.isEmpty())
    {
        return;
    }

    QString newText;

    // Toggle bold
    QRegExp strikeoutRx("~{2}.*~{2}");

    if (text.indexOf(strikeoutRx) > -1)
    {
        newText = text.mid(2, text.size() - 4);
    }
    else
    {
        newText = "~~" + text + "~~";
    }

    currentCursor.insertText(newText);
}


void MarkdownEditor::undo()
{
    mUi->textEdit->undo();
}


void MarkdownEditor::redo()
{
    mUi->textEdit->redo();
}


void MarkdownEditor::insertHorizontalLine()
{
    QTextCursor currentCursor = mUi->textEdit->textCursor();
    currentCursor.insertBlock();
    currentCursor.insertText("-------------------------------------------");
    currentCursor.insertBlock();
}


void MarkdownEditor::insertModifyLink()
{
    QTextCursor currentCursor = mUi->textEdit->textCursor();
    QString selectedText = currentCursor.selectedText();

    InsertModifyLinkDialog dialog(this);
    dialog.fromMarkdownFormat(selectedText);

    if (dialog.exec())
    {
        QString fmt = dialog.toMarkdownFormat();

        if (!fmt.isEmpty())
        {
            currentCursor.insertText(fmt);
        }
    }
}
