#include "markdowneditorhighlighter.h"

MarkdownEditorHighlighter::MarkdownEditorHighlighter(QTextDocument* parent) :
    QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // bold
    mBoldStartRx = QRegExp("\\*\\*.");
    mBoldEndRx = QRegExp(".\\*\\*");

    mBoldFmt.setForeground(Qt::black);
    mBoldFmt.setFontWeight(QFont::ExtraBold);

    // italic
    mItalicStartRx = QRegExp("\\*.");
    mItalicEndRx = QRegExp(".\\*");

    mItalicFmt.setForeground(Qt::black);
    mItalicFmt.setFontItalic(true);

    // h1
    rule.pattern = QRegExp("^# .*$");
    rule.format = mBoldFmt;
    highlightingRules.append(rule);

    // h2
    rule.pattern = QRegExp("^## .*$");
    rule.format = mBoldFmt;
    highlightingRules.append(rule);

    // h3
    rule.pattern = QRegExp("^### .*$");
    rule.format = mBoldFmt;
    highlightingRules.append(rule);

    // h4
    rule.pattern = QRegExp("^#### .*$");
    rule.format = mBoldFmt;
    highlightingRules.append(rule);

    // h5
    rule.pattern = QRegExp("^##### .*$");
    rule.format = mBoldFmt;
    highlightingRules.append(rule);
}


void MarkdownEditorHighlighter::highlightBlock(const QString& text)
{
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
    distanceHightlightBlockHelper(text, mBoldStartRx, mBoldEndRx, mBoldFmt);

    // italic
    distanceHightlightBlockHelper(text, mItalicStartRx, mItalicEndRx, mItalicFmt);
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
