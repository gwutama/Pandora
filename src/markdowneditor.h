#ifndef MARKDOWNEDITOR_H
#define MARKDOWNEDITOR_H

#include <QWidget>

namespace Ui
{
class MarkdownEditor;
}

/**
 * @brief The MarkdownEditor class
 * Resources:
 * http://doc.qt.io/qt-5/qtwidgets-richtext-syntaxhighlighter-example.html
 */
class MarkdownEditor : public QWidget
{
    Q_OBJECT

public:
    explicit MarkdownEditor(QWidget* parent = 0);
    ~MarkdownEditor();

public slots:
    void newFile();
    void openFile(const QString& path = QString());

private:
    void setupEditor();

private:
    Ui::MarkdownEditor* ui;
};

#endif // MARKDOWNEDITOR_H
