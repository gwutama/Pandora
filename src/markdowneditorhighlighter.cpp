#include "markdowneditorhighlighter.h"

MarkdownEditorHighlighter::MarkdownEditorHighlighter(QTextDocument* parent) :
    QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // Formats
    mDefaultFmt.setForeground(QColor(68, 68, 68)); // 444444

    mEmphasisFmt.setForeground(QColor(68, 68, 68)); // 444444
    mEmphasisFmt.setFontWeight(QFont::Bold);

    mEmphasisMoreFmt.setForeground(QColor(68, 68, 68)); // 444444
    mEmphasisMoreFmt.setFontWeight(QFont::Bold);
    mEmphasisMoreFmt.setFontItalic(true);

    mHeadingFmt.setForeground(Qt::black);
    mHeadingFmt.setFontWeight(QFont::Bold);
    mHeadingFmt.setFontLetterSpacingType(QFont::PercentageSpacing);
    mHeadingFmt.setFontLetterSpacing(130);

    mItalicFmt.setForeground(QColor(68, 68, 68)); // 444444
    mItalicFmt.setFontItalic(true);

    mStrikeOutFmt.setForeground(QColor(204, 204, 204)); // CCCCCC
    mStrikeOutFmt.setFontStrikeOut(true);

    mInvertFmt.setBackground(QColor(221, 221, 221)); // DDDDDD
    mInvertFmt.setForeground(QColor(204, 204, 204)); // CCCCCC

    mLinkFmt.setForeground(Qt::black); // 000000
    mLinkFmt.setFontItalic(true);


    // h1
    rule.pattern = QRegExp("^# .*$");
    rule.format = mHeadingFmt;
    highlightingRules.append(rule);

    // h2
    rule.pattern = QRegExp("^## .*$");
    rule.format = mHeadingFmt;
    highlightingRules.append(rule);

    // h3
    rule.pattern = QRegExp("^### .*$");
    rule.format = mHeadingFmt;
    highlightingRules.append(rule);

    // h4
    rule.pattern = QRegExp("^#### .*$");
    rule.format = mEmphasisFmt;
    highlightingRules.append(rule);

    // h5
    rule.pattern = QRegExp("^##### .*$");
    rule.format = mEmphasisFmt;
    highlightingRules.append(rule);

    // table lines and horizontal rules
    rule.pattern = QRegExp("^--[ -]+$");
    rule.format = mInvertFmt;
    highlightingRules.append(rule);

    // asterisk horizontal rules
    rule.pattern = QRegExp("^\\*\\*[\\*]+$");
    rule.format = mInvertFmt;
    highlightingRules.append(rule);

    // strike-out
    // must be done here because it clashes with delimiter block
    rule.pattern = QRegExp("~~(?!~).*~~");
    rule.format = mStrikeOutFmt;
    highlightingRules.append(rule);

    // link
    rule.pattern = QRegExp("!?\\[.*\\]\\(.*\\)");
    rule.format = mLinkFmt;
    highlightingRules.append(rule);
}


void MarkdownEditorHighlighter::highlightBlock(const QString& text)
{
    setFormat(0, text.length(), mDefaultFmt); // apply default format for all text first

    foreach (const HighlightingRule& rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);

        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    // bold
    distanceHightlightBlockHelper(text, cBoldAsteriskStartRx, cBoldAsteriskEndRx, mEmphasisFmt);
    // TODO: Doesn't work!
    distanceHightlightBlockHelper(text, cBoldUnderlineStartRx, cBoldUnderlineEndRx, mEmphasisFmt);

    // italic
    distanceHightlightBlockHelper(text, cItalicAsteriskStartRx, cItalicAsteriskEndRx, mItalicFmt);
    distanceHightlightBlockHelper(text, cItalicUnderlineStartRx, cItalicUnderlineEndRx, mItalicFmt);

    // bold italic
    distanceHightlightBlockHelper(text, cBoldItalicAsteriskStartRx, cBoldItalicAsteriskEndRx,
                                  mEmphasisMoreFmt);
    distanceHightlightBlockHelper(text, cBoldItalicUnderlineStartRx, cBoldItalicUnderlineEndRx,
                                  mEmphasisMoreFmt);

    // strike-out
    // Cannot be done here because it clashes with delimiter block
    // distanceHightlightBlockHelper(text, cStrikeOutStartRx, cStrikeOutEndRx, mStrikeOutFmt);

    // delimiter block
    distanceHightlightBlockHelper(text, cDelimiterStartRx, cDelimiterEndRx, mInvertFmt);
}


void MarkdownEditorHighlighter::distanceHightlightBlockHelper(const QString& text,
                                                              const QRegExp& startPattern,
                                                              const QRegExp& endPattern,
                                                              const QTextCharFormat format)
{
    setCurrentBlockState(0);

    int startIndex = 0;

    if (previousBlockState() != 1)
    {
        startIndex = startPattern.indexIn(text);
    }

    while (startIndex >= 0)
    {
        int endIndex = endPattern.indexIn(text, startIndex);
        int textLen;

        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            textLen = text.length() - startIndex;
        }
        else
        {
            textLen = endIndex - startIndex + endPattern.matchedLength();
        }

        setFormat(startIndex, textLen, format);
        startIndex = startPattern.indexIn(text, startIndex + textLen);
    }
}
