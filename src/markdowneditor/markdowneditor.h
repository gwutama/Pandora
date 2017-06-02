#ifndef MARKDOWNEDITOR_H
#define MARKDOWNEDITOR_H

#include <QWidget>
#include <QSharedPointer>
#include <QTimer>
#include <QTextEdit>
#include <QDialog>
#include "common/appconfig.h"
#include "markdowneditorhighlighter.h"
#include "imagepreviewdialog.h"
#include "documentstatisticsdialog.h"

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
    void setMargin(unsigned int size);

public slots:
    void refocusEditor();
    bool open();
    void close();
    bool save();
    bool saveAs(const QString& path);
    void showFindReplaceWidget();

    inline void setNarrowMargin()
    { setMargin(80); }

    inline void setWideMargin()
    { setMargin(150); }

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
    void checkContentChanged();

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

private:
    void setupEditor();
    QString parseFirstPathInText(const QString& text);
    bool openFile(const QString& path = QString());
    void toggleHeadingHelper(int level);

private:
    Ui::MarkdownEditor* mUi;
    QSharedPointer<AppConfig> mConfig;
    QSharedPointer<MarkdownEditorHighlighter> mHighlighter;
    QTimer mContentChangeTimer;
    QString mOldContent;
    QList<QTextEdit::ExtraSelection> mMatchTextSelections;
    ImagePreviewDialog* mImagePreviewDialog;
    DocumentStatisticsDialog* mDocStatsDialog;

    static const char* sTag;
};

#endif // MARKDOWNEDITOR_H
