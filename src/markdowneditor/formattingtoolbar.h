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
    void boldSelectionButtonClicked();
    void italicSelectionButtonClicked();
    void strikeoutSelectionButtonClicked();

private:
    Ui::FormattingToolbar* mUi;
};

#endif // FORMATTINGTOOLBAR_H
