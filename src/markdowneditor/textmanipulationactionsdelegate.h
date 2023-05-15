#ifndef TEXTMANIPULATIONACTIONSDELEGATE_H
#define TEXTMANIPULATIONACTIONSDELEGATE_H

#include <QObject>
#include "markdowntextedit.h"

class TextManipulationActionsDelegate : public QObject
{
    Q_OBJECT

public:
    explicit TextManipulationActionsDelegate(MarkdownTextEdit* textEdit,
                                             QObject* parent = nullptr);
    virtual ~TextManipulationActionsDelegate();

public slots:
    void undo();
    void redo();

    void toggleSelectionBold();
    void toggleSelectionItalic();
    void toggleSelectionStrikeout();

    void insertHorizontalLine();
    void insertModifyLink();
    void insertModifyImage();

    void toggleSelectionUnorderedList();
    void toggleSelectionOrderedList();

    void toggleSelectionBlockquote();

    inline void toggleH1()
    { toggleHeadingHelper(1); }

    inline void toggleH2()
    { toggleHeadingHelper(2); }

    inline void toggleH3()
    { toggleHeadingHelper(3); }

private:
    void toggleHeadingHelper(int level);

private:
    MarkdownTextEdit* mTextEdit;
};

#endif // TEXTMANIPULATIONACTIONSDELEGATE_H
