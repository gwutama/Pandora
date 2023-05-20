#ifndef MARKDOWNEDITORHIGHLIGHTER_H
#define MARKDOWNEDITORHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegExp>

class QTextDocument;

/**
 * @brief The MarkdownEditorHighlighter class
 *
 * Resource:
 * http://doc.qt.io/qt-5/qtwidgets-richtext-syntaxhighlighter-example.html
 * http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html
 */
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

    const QRegExp cBoldAsteriskStartRx = QRegExp("\\*{2}\\S");
    const QRegExp cBoldAsteriskEndRx = QRegExp("\\S\\*{2}");
    const QRegExp cBoldUnderlineStartRx = QRegExp("{2}_\\S)");
    const QRegExp cBoldUnderlineEndRx = QRegExp("\\S_{2}");
    const QRegExp cItalicAsteriskStartRx = QRegExp("\\*\\S");
    const QRegExp cItalicAsteriskEndRx = QRegExp("\\S\\*");
    const QRegExp cItalicUnderlineStartRx = QRegExp("_\\S");
    const QRegExp cItalicUnderlineEndRx = QRegExp("\\S_");
    const QRegExp cBoldItalicAsteriskStartRx = QRegExp("\\*{3}\\S");
    const QRegExp cBoldItalicAsteriskEndRx = QRegExp("\\S\\*{3}");
    const QRegExp cBoldItalicUnderlineStartRx = QRegExp("___\\S");
    const QRegExp cBoldItalicUnderlineEndRx = QRegExp("\\S___");
    const QRegExp cDelimiterStartRx = QRegExp("~~~[~]*[A-Za-z0-9]*");
    const QRegExp cDelimiterEndRx = QRegExp("~~~[~]*");

    QTextCharFormat mDefaultFmt;
    QTextCharFormat mEmphasisFmt;
    QTextCharFormat mEmphasisMoreFmt;
    QTextCharFormat mHeadingFmt;
    QTextCharFormat mItalicFmt;
    QTextCharFormat mStrikeOutFmt;
    QTextCharFormat mInvertFmt;
    QTextCharFormat mLinkFmt;
};

#endif // MARKDOWNEDITORHIGHLIGHTER_H
