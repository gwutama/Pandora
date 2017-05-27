#include "markdowneditor.h"
#include "ui_markdowneditor.h"
#include <QTextEdit>
#include <QFileInfo>
#include <QDebug>
#include <QShortcut>

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

    connect(mUi->findReplaceWidget, &FindReplaceWidget::textChanged,
            this, &MarkdownEditor::highlightFoundText);
    connect(mUi->findReplaceWidget, &FindReplaceWidget::nextButtonClicked,
            this, &MarkdownEditor::goToNextFoundText);
    connect(mUi->findReplaceWidget, &FindReplaceWidget::previousButtonClicked,
            this, &MarkdownEditor::goToPreviousFoundText);

    connect(&mContentChangeTimer, &QTimer::timeout, this, &MarkdownEditor::checkContentChanged);

    QShortcut* escKeyShortcut = new QShortcut(Qt::Key_Escape, parent);
    connect(escKeyShortcut, &QShortcut::activated, this, &MarkdownEditor::onEscKeyActivated);
}


MarkdownEditor::~MarkdownEditor()
{
    delete mUi;
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
        removeHighlightFoundText();
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
    doc->setDocumentMargin(70);

    file.close();
    return true;
}


void MarkdownEditor::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(18);
    mUi->textEdit->setFont(font);

    QFontMetrics metrics(font);
    mUi->textEdit->setTabStopWidth(4 * metrics.width(' ')); // replace tab with 4 spaces

    MarkdownEditorHighlighter* mhPtr = new MarkdownEditorHighlighter(mUi->textEdit->document());
    mHighlighter = QSharedPointer<MarkdownEditorHighlighter>(mhPtr);
}


void MarkdownEditor::showFindReplaceWidget()
{
    if (isEnabled())
    {
        mUi->findReplaceWidget->show();
        mUi->findReplaceWidget->setFocus();
    }
}


void MarkdownEditor::highlightFoundText(const QString& searchString)
{

    if (searchString.isNull())
    {
        return;
    }

    // https://stackoverflow.com/questions/7624196/find-all-strings-wanted-and-select-them-with-qplaintexteditsetextraselections
    qDebug() << sTag << "Searching string in document:" << searchString;

    mFoundTextSelections.clear();
    mUi->textEdit->moveCursor(QTextCursor::Start);

    QTextCharFormat colorFormat;
    colorFormat.setBackground(Qt::yellow);

    while (mUi->textEdit->find(searchString))
    {
        QTextEdit::ExtraSelection extra;
        extra.cursor = mUi->textEdit->textCursor();
        extra.format = colorFormat;
        mFoundTextSelections.append(extra);
    }

    mUi->textEdit->setExtraSelections(mFoundTextSelections);
    mUi->findReplaceWidget->setFoundNumber(mFoundTextSelections.size());
}


void MarkdownEditor::removeHighlightFoundText()
{
    mFoundTextSelections.clear();
    mUi->textEdit->setExtraSelections(mFoundTextSelections);
}


void MarkdownEditor::goToNextFoundText()
{
    if (!mFoundTextSelections.size())
    {
        qDebug() << sTag << "Text not found. Not going to next found text";
        return;
    }

    QTextCursor cursor = mUi->textEdit->textCursor();

    foreach (QTextEdit::ExtraSelection selection, mFoundTextSelections)
    {
        QTextCursor selectionCursor = selection.cursor;

        if (selectionCursor.position() > cursor.position())
        {
            mUi->textEdit->setTextCursor(selectionCursor);
            return;
        }
    }

    // Cursor hasn't been set. cycle to first selection in the list.
    QTextCursor firstPos = mFoundTextSelections.at(0).cursor;
    mUi->textEdit->setTextCursor(firstPos);
}


void MarkdownEditor::goToPreviousFoundText()
{
    if (!mFoundTextSelections.size())
    {
        qDebug() << sTag << "Text not found. Not going to previous found text";
        return;
    }

    QTextCursor cursor = mUi->textEdit->textCursor();
    QList<QTextEdit::ExtraSelection>::iterator iter;

    for(iter = mFoundTextSelections.end() - 1; iter != mFoundTextSelections.begin() - 1; --iter)
    {
        QTextCursor selectionCursor = iter->cursor;

        if (selectionCursor.position() < cursor.position())
        {
            mUi->textEdit->setTextCursor(selectionCursor);
            return;
        }
    }

    // Cursor hasn't been set. cycle to last selection in the list.
    QTextCursor lastPos = mFoundTextSelections.at(mFoundTextSelections.size() - 1).cursor;
    mUi->textEdit->setTextCursor(lastPos);
}
