#ifndef MARKDOWNTEXTEDIT_H
#define MARKDOWNTEXTEDIT_H

#include <QPlainTextEdit>
#include <QStringList>

class MarkdownTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit MarkdownTextEdit(QWidget* parent = 0);
    virtual ~MarkdownTextEdit();

public slots:
    void showContextMenu(const QStringList& suggestions,
                         const QPoint& point);

protected slots:
    void keyPressEvent(QKeyEvent* event);

private slots:
    void onSuggestionActionTriggered();

signals:
    void replaceSelection(const QString& word);

private:
    void indent(QKeyEvent* event);
    void outdent(QKeyEvent* event);

private:
    QList<QAction*> mSuggestionActions;
    static const char* sTag;
};

#endif // MARKDOWNTEXTEDIT_H
