#ifndef FORMATTINGTOOLBAR_H
#define FORMATTINGTOOLBAR_H

#include <QWidget>

namespace Ui
{
class FormattingToolbar;
}

class FormattingToolbar : public QWidget
{
    Q_OBJECT

public:
    explicit FormattingToolbar(QWidget* parent = 0);
    ~FormattingToolbar();

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

private:
    Ui::FormattingToolbar* mUi;
};

#endif // FORMATTINGTOOLBAR_H
