#ifndef MARKDOWNTEXTEDIT_H
#define MARKDOWNTEXTEDIT_H

#include <QPlainTextEdit>
#include <QStringList>
#include <QTimer>

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
    void mousePressEvent(QMouseEvent* event);

private slots:
    void onSuggestionActionTriggered();
    void emitVerticalScrollEnd();

signals:
    void suggestionActionTriggered(const QString& word);
    void verticalScrollEnd(int position);

private:
    void indent(QKeyEvent* event);
    void outdent(QKeyEvent* event);

private:
    QList<QAction*> mSuggestionActions;
    QTimer mVerticalScrollTimer;
    int mVerticalScrollPos;
    static const char* sTag;
};

#endif // MARKDOWNTEXTEDIT_H
