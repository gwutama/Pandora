#ifndef FINDREPLACEACTIONSDELEGATE_H
#define FINDREPLACEACTIONSDELEGATE_H

#include <QObject>
#include "markdowntextedit.h"
#include "findreplacewidget.h"

class FindReplaceActionsDelegate : public QObject
{
    Q_OBJECT

public:
    explicit FindReplaceActionsDelegate(MarkdownTextEdit* textEdit,
                                        FindReplaceWidget* findReplaceWidget,
                                        QObject* parent = nullptr);
    virtual ~FindReplaceActionsDelegate();

public slots:
    void removeHighlightMatches();

private slots:
    void searchAndHighlightMatches();
    bool goToNextMatch();
    bool goToPreviousMatch();
    bool replaceMatch();
    bool replaceMatchGoToNextOne();
    bool replaceMatches();

private:
    bool goToNthMatch(unsigned int index);
    bool goToNearestMatch(const QTextCursor& cursor);

private:
    MarkdownTextEdit* mTextEdit;
    FindReplaceWidget* mFindReplaceWidget;
    QList<QTextEdit::ExtraSelection> mMatchTextSelections;

    static const char* sTag;
};

#endif // FINDREPLACEACTIONSDELEGATE_H
