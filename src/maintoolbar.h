#ifndef FORMATTINGTOOLBAR_H
#define FORMATTINGTOOLBAR_H

#include <QWidget>

namespace Ui
{
class FormattingToolbar;
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

    // Others
    void horizontalLineButtonClicked(bool checked);

private:
    Ui::FormattingToolbar* mUi;
};

#endif // FORMATTINGTOOLBAR_H
