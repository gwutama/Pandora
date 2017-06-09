#ifndef MARKDOWNEDITOR_H
#define MARKDOWNEDITOR_H

#include <QWidget>
#include <QSharedPointer>
#include <QTextEdit>
#include "common/appconfig.h"
#include "imagepreviewdialog.h"
#include "documentstatisticsdialog.h"
#include "spellcheck.h"
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

public slots:
    void refocusEditor();
    bool open();
    void close();
    bool save();
    bool saveAs(const QString& path);
    void showFindReplaceWidget();

    void setNarrowMargin();
    void setWideMargin();

    void increaseFontSize();
    void decreaseFontSize();

    void toggleSelectionBold();
    void toggleSelectionItalic();
    void toggleSelectionStrikeout();

    inline void toggleH1()
    { toggleHeadingHelper(1); }

    inline void toggleH2()
    { toggleHeadingHelper(2); }

    inline void toggleH3()
    { toggleHeadingHelper(3); }

    void undo();
    void redo();

    void insertHorizontalLine();
    void insertModifyLink();
    void insertModifyImage();

    void toggleSelectionUnorderedList();
    void toggleSelectionOrderedList();

    void toggleSelectionBlockquote();

    inline void showDocumentStatsDialog()
    { mDocStatsDialog->show(); }

signals:
    void contentChanged(const QString& content);

private slots:
    void onEscKeyActivated();
    void onMagicShortcutActivated();
    void toggleImagePreviewModal();

    // Find and replace methods
    void searchAndHighlightMatches();
    void removeHighlightMatches();
    bool goToNextMatch();
    bool goToPreviousMatch();
    bool goToNthMatch(unsigned int index);
    bool goToNearestMatch(const QTextCursor& cursor);
    bool replaceMatch();
    bool replaceMatchGoToNextOne();
    bool replaceMatches();

    // Spell checker and grammar tools
    void grammarCheckDocument();
    void grammarCheckFinished();

private:
    void setupEditor();
    QString parseFirstPathInText(const QString& text);
    void toggleHeadingHelper(int level);

private:
    Ui::MarkdownEditor* mUi;
    QSharedPointer<AppConfig> mConfig;
    QList<QTextEdit::ExtraSelection> mMatchTextSelections;
    QList<QTextEdit::ExtraSelection> mGrammarCheckSelections;
    ImagePreviewDialog* mImagePreviewDialog;
    DocumentStatisticsDialog* mDocStatsDialog;
    QSharedPointer<LanguageTool> mLanguageTool;

    static const char* sTag;
};

#endif // MARKDOWNEDITOR_H
