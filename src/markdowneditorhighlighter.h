#ifndef MARKDOWNEDITORHIGHLIGHTER_H
#define MARKDOWNEDITORHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class QTextDocument;

class MarkdownEditorHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    MarkdownEditorHighlighter(QTextDocument* parent = 0);

protected:
    void highlightBlock(const QString& text) override;

private:
    void distanceHightlightBlockHelper(const QString& text,
                                       const QRegExp& startPattern,
                                       const QRegExp& endPattern,
                                       const QTextCharFormat format);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

    QRegExp mBoldStartRx;
    QRegExp mBoldEndRx;
    QRegExp mItalicStartRx;
    QRegExp mItalicEndRx;

    QTextCharFormat mBoldFmt;
    QTextCharFormat mItalicFmt;
};

#endif // MARKDOWNEDITORHIGHLIGHTER_H
