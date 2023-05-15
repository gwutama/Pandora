#ifndef PROOFREADACTIONSDELEGATEBASE_H
#define PROOFREADACTIONSDELEGATEBASE_H

#include <QObject>
#include "markdowntextedit.h"

class ProofreadActionsDelegateBase : public QObject
{
    Q_OBJECT

public:
    explicit ProofreadActionsDelegateBase(MarkdownTextEdit* textEdit,
                                          QObject* parent = nullptr);
    virtual ~ProofreadActionsDelegateBase();

    inline QList<QTextEdit::ExtraSelection> selections()
    { return mSelections; }

public slots:
    virtual void checkVisibleText();
    virtual void checkDocument();

protected slots:
    virtual void showContextMenu(const QPoint& point) = 0;
    virtual void replaceSelectionWithTextReplacement() = 0;

protected:
    virtual void runVisibleTextCheck(const QString& text, int startPos, int endPos) = 0;
    virtual void runDocumentCheck(const QString& text) = 0;
    virtual void showContextMenuWithSuggestions(const QPoint& point,
                                                const QStringList& suggestions = QStringList());

protected:
    MarkdownTextEdit* mTextEdit;
    QList<QTextEdit::ExtraSelection> mSelections;
    QList<QAction*> mSuggestionActions;

private:
    static const char* sTag;
};

#endif // PROOFREADACTIONSDELEGATEBASE_H
