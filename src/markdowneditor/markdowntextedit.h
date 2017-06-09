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
    void checkVerticalScroll();
    void checkTextChanged();

signals:
    void suggestionActionTriggered(const QString& word);
    void laxVerticalScrollEnd(int position);
    void laxTextChanged(const QString& text);

private:
    void indent(QKeyEvent* event);
    void outdent(QKeyEvent* event);

private:
    QList<QAction*> mSuggestionActions;
    QTimer mVerticalScrollTimer;
    QTimer mContentChangeTimer;
    int mVerticalScrollPos;
    QString mOldContent;
    static const char* sTag;
};

#endif // MARKDOWNTEXTEDIT_H
