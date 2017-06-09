#ifndef MARKDOWNEDITOR_H
#define MARKDOWNEDITOR_H

#include <QWidget>
#include <QSharedPointer>
#include <QTextEdit>
#include "common/appconfig.h"
#include "textmanipulationactionsdelegate.h"
#include "findreplaceactionsdelegate.h"
#include "spellcheckactionsdelegate.h"
#include "imagepreviewdialog.h"
#include "documentstatisticsdialog.h"
#include "languagetool.h"

namespace Ui
{
class MarkdownEditor;
}

/**
 * @brief The MarkdownEditor class
 * Resources:
 * http://doc.qt.io/qt-5/qtwidgets-richtext-syntaxhighlighter-example.html
 * http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html
 */
class MarkdownEditor : public QWidget
{
    Q_OBJECT

public:
    explicit MarkdownEditor(QSharedPointer<AppConfig> config,
                            QWidget* parent = 0);
    ~MarkdownEditor();

    QString content();

    QSharedPointer<TextManipulationActionsDelegate> textManipulationActions()
    { return mTextManipulationActions; }

public slots:
    void refocusEditor();
    bool open();
    void close();

    inline bool save()
    { return saveAs(mConfig->markdownFile()); }

    bool saveAs(const QString& path);
    void showFindReplaceWidget();

    void setNarrowMargin();
    void setWideMargin();

    void increaseFontSize();
    void decreaseFontSize();

    inline void showDocumentStatsDialog()
    { mDocStatsDialog->show(); }

signals:
    void contentChanged(const QString& content);

private slots:
    void onEscKeyActivated();
    void onMagicShortcutActivated();
    void toggleImagePreviewModal();

    // Spell checker and grammar tools
    void grammarCheckDocument();
    void grammarCheckFinished();

private:
    void setupEditor();
    QString parseFirstPathInText(const QString& text);

private:
    Ui::MarkdownEditor* mUi;
    QSharedPointer<AppConfig> mConfig;
    QSharedPointer<TextManipulationActionsDelegate> mTextManipulationActions;
    QSharedPointer<FindReplaceActionsDelegate> mFindReplaceActions;
    QSharedPointer<SpellCheckActionsDelegate> mSpellCheckActions;
    QList<QTextEdit::ExtraSelection> mGrammarCheckSelections;
    ImagePreviewDialog* mImagePreviewDialog;
    DocumentStatisticsDialog* mDocStatsDialog;
    QSharedPointer<LanguageTool> mLanguageTool;

    static const char* sTag;
};

#endif // MARKDOWNEDITOR_H
