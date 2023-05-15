#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

#include <QWidget>

namespace Ui
{
class MainToolbar;
}

class MainToolbar : public QWidget
{
    Q_OBJECT

public:
    explicit MainToolbar(QWidget* parent = 0);
    ~MainToolbar();

signals:
    // Document
    void newDocumentButtonClicked(bool checked);
    void saveDocumentButtonClicked(bool checked);

    // Undo/redo
    void undoButtonClicked(bool checked);
    void redoButtonClicked(bool checked);

    // Formatting
    void boldSelectionButtonClicked(bool checked);
    void italicSelectionButtonClicked(bool checked);
    void strikeoutSelectionButtonClicked(bool checked);

    // Heading
    void h1ButtonClicked(bool checked);
    void h2ButtonClicked(bool checked);
    void h3ButtonClicked(bool checked);

    // Others
    void horizontalLineButtonClicked(bool checked);
    void linkButtonClicked(bool checked);
    void imageButtonClicked(bool clicked);
    void orderedListButtonClicked(bool clicked);
    void unorderedListButtonClicked(bool clicked);
    void blockquoteButtonClicked(bool clicked);

private:
    Ui::MainToolbar* mUi;
};

#endif // MAINTOOLBAR_H
