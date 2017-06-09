#include "markdowneditor.h"
#include "ui_markdowneditor.h"
#include <QDebug>
#include <QShortcut>
#include "insertmodifylinkdialog.h"
#include "insertmodifyimagedialog.h"

const char* MarkdownEditor::sTag = "[MarkdownEditor]";

MarkdownEditor::MarkdownEditor(QSharedPointer<AppConfig> config,
                               QWidget* parent) :
    QWidget(parent),
    mConfig(config),
    mUi(new Ui::MarkdownEditor),
    mImagePreviewDialog(new ImagePreviewDialog(this))
{
    mUi->setupUi(this);
    setupEditor();

    // Text manipulation actions
    auto textActionsPtr = new TextManipulationActionsDelegate(mUi->textEdit);
    mTextManipulationActions = QSharedPointer<TextManipulationActionsDelegate>(textActionsPtr);

    mDocStatsDialog = new DocumentStatisticsDialog(mUi->textEdit->document(), this);
    mUi->findReplaceWidget->hide();

    // Setup language tool
    LanguageTool* ltPtr = new LanguageTool(QUrl("http://localhost:8080/v2/check"));
    mLanguageTool = QSharedPointer<LanguageTool>(ltPtr);
    connect(mLanguageTool.data(), &LanguageTool::suggestionsReady,
            this, &MarkdownEditor::grammarCheckFinished);

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

    connect(mUi->textEdit, &MarkdownTextEdit::laxTextChanged,
            this, &MarkdownEditor::contentChanged);

    QShortcut* escKeyShortcut = new QShortcut(Qt::Key_Escape, parent);
    connect(escKeyShortcut, &QShortcut::activated, this, &MarkdownEditor::onEscKeyActivated);

    QShortcut* magicShortcut = new QShortcut(QKeySequence(tr("Ctrl+M")), parent);
    connect(magicShortcut, &QShortcut::activated, this, &MarkdownEditor::onMagicShortcutActivated);
}


MarkdownEditor::~MarkdownEditor()
{
    delete mUi;
}


void MarkdownEditor::setupEditor()
{
    QFont font = mUi->textEdit->font();
    font.setPointSize(mConfig->editorFontPointSize());
    mUi->textEdit->setFont(font);
    setNarrowMargin(); // default margin

    auto spellCheck = QSharedPointer<SpellCheck>(new SpellCheck(mConfig));
    mUi->textEdit->setSpellCheck(spellCheck);
}


QString MarkdownEditor::content()
{
    return mUi->textEdit->document()->toPlainText();
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


void MarkdownEditor::onMagicShortcutActivated()
{
    toggleImagePreviewModal();
}


void MarkdownEditor::toggleImagePreviewModal()
{
    if (!mImagePreviewDialog->isHidden())
    {
        mImagePreviewDialog->hide();
    }

    QTextCursor cursor = mUi->textEdit->textCursor();
    int cursorPosInBlock = cursor.positionInBlock();
    cursor.select(QTextCursor::BlockUnderCursor);
    QString selectedText = cursor.selectedText();

    // detect whether current cursor is within a path
    QString path = parseFirstPathInText(selectedText);

    int idxStart = selectedText.indexOf(path);
    int idxEnd = idxStart + path.size();

    if (!path.isEmpty() && idxStart != -1 &&
            cursorPosInBlock >= idxStart && cursorPosInBlock <= idxEnd)
    {
        qDebug() << sTag << "Detected a path in block:" << path;
        mImagePreviewDialog->previewImage(path);
    }
}


QString MarkdownEditor::parseFirstPathInText(const QString& text)
{
    QString firstMatch;
    QRegExp pathRx(".*((/?[\\w ]/?)+/[\\w ]+\\.(gif|jpeg|jpg|png)).*"); // *nix only!

    if (pathRx.indexIn(text) > -1) // we have match
    {
        QStringList matches = pathRx.capturedTexts();

        if (matches.size())
        {
            firstMatch = matches.at(1);
        }
    }

    return firstMatch;
}


void MarkdownEditor::refocusEditor()
{
    mUi->textEdit->setFocus();
}


bool MarkdownEditor::open()
{
    QFile file(mConfig->markdownFile());

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qWarning() << sTag << "Cannot open file for reading:" << mConfig->markdownFile();
        return false;
    }

    QByteArray ba = file.readAll();
    QTextDocument* doc = mUi->textEdit->document();
    doc->setPlainText(ba);
    mUi->textEdit->moveCursor(QTextCursor::Start);
    file.close();

    mImagePreviewDialog->hide();

    mUi->textEdit->spellcheckVisibleText();

    // Emit signals so that mainwindow picks the signal up and tell markdownpreview
    // to generate preview
    emit contentChanged(content());
    return true;
}


void MarkdownEditor::close()
{
    mUi->textEdit->clear();
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


void MarkdownEditor::setNarrowMargin()
{
    mUi->textEdit->setMargin(80);
}


void MarkdownEditor::setWideMargin()
{
    mUi->textEdit->setMargin(150);
}


void MarkdownEditor::increaseFontSize()
{
    mUi->textEdit->increaseFontSize();
    mConfig->setEditorFontPointSize(mUi->textEdit->font().pointSize());
    mConfig->save();
}


void MarkdownEditor::decreaseFontSize()
{
    mUi->textEdit->decreaseFontSize();
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


void MarkdownEditor::grammarCheckDocument()
{
    mLanguageTool->check(content());
}


void MarkdownEditor::grammarCheckFinished()
{
    mGrammarCheckSelections.clear();
    QTextCursor currentCursor = mUi->textEdit->textCursor();
    mUi->textEdit->moveCursor(QTextCursor::Start);

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

    mUi->textEdit->setExtraSelections(mGrammarCheckSelections);
    mUi->textEdit->setTextCursor(currentCursor);
}
