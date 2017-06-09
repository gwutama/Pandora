#include "markdowneditor.h"
#include "ui_markdowneditor.h"
#include <QDebug>
#include <QShortcut>

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

    // Find replace actions
    auto findActionsPtr = new FindReplaceActionsDelegate(mUi->textEdit, mUi->findReplaceWidget);
    mFindReplaceActions = QSharedPointer<FindReplaceActionsDelegate>(findActionsPtr);

    // Spell check actions
    auto spellCheck = QSharedPointer<SpellCheck>(new SpellCheck(mConfig));
    auto spellCheckActionsPtr = new SpellCheckActionsDelegate(mUi->textEdit, spellCheck);
    mSpellCheckActions = QSharedPointer<SpellCheckActionsDelegate>(spellCheckActionsPtr);

    // Document statistics dialog
    mDocStatsDialog = new DocumentStatisticsDialog(mUi->textEdit->document(), this);

    // Grammar check actions
    auto ltPtr = new LanguageTool(QUrl("http://localhost:8080/v2/check"));
    auto languageTool = QSharedPointer<LanguageTool>(ltPtr);
    auto grammarCheckActionsPtr = new GrammarCheckActionsDelegate(mUi->textEdit, languageTool);
    mGrammarCheckActions = QSharedPointer<GrammarCheckActionsDelegate>(grammarCheckActionsPtr);

    // Text edit signals slots
    connect(mUi->textEdit, &MarkdownTextEdit::laxTextChanged,
            this, &MarkdownEditor::contentChanged);
    connect(mUi->textEdit, &MarkdownTextEdit::laxTextChanged,
            mSpellCheckActions.data(), &SpellCheckActionsDelegate::spellcheckVisibleText);
    connect(mUi->textEdit, &MarkdownTextEdit::laxVerticalScrollEnd,
            mSpellCheckActions.data(), &SpellCheckActionsDelegate::spellcheckVisibleText);
    connect(this, &MarkdownEditor::contentChanged,
            mSpellCheckActions.data(), &SpellCheckActionsDelegate::spellcheckVisibleText);

    // Escape shortcuts
    QShortcut* escKeyShortcut = new QShortcut(Qt::Key_Escape, parent);
    connect(escKeyShortcut, &QShortcut::activated, this, &MarkdownEditor::onEscKeyActivated);
    connect(escKeyShortcut, &QShortcut::activated,
            mFindReplaceActions.data(), &FindReplaceActionsDelegate::removeHighlightMatches);

    // Image shortcut
    QShortcut* imgShortcut = new QShortcut(QKeySequence(tr("Ctrl+M")), parent);
    connect(imgShortcut, &QShortcut::activated, this, &MarkdownEditor::toggleImagePreviewModal);

    // Misc
    mUi->findReplaceWidget->hide();
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
    }
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

    // Emit signals so that
    // 1. mainwindow picks the signal up and tell markdownpreview to generate preview
    // 2. SpellCheckActionsDelegate picks the signal up and runs spell check
    emit contentChanged(content());
    return true;
}


void MarkdownEditor::close()
{
    mUi->textEdit->clear();
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
