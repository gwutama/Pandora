#ifndef MARKDOWNTEXTEDIT_H
#define MARKDOWNTEXTEDIT_H

#include <QPlainTextEdit>

class MarkdownTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit MarkdownTextEdit(QWidget* parent = 0);
    virtual ~MarkdownTextEdit();

protected slots:
    void keyPressEvent(QKeyEvent* event);

private:
    void indent(QKeyEvent* event);
    void outdent(QKeyEvent* event);

private:
    static const char* sTag;
};

#endif // MARKDOWNTEXTEDIT_H
