#ifndef FINDREPLACEWIDGET_H
#define FINDREPLACEWIDGET_H

#include <QWidget>

namespace Ui
{
class FindReplaceWidget;
}

class FindReplaceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FindReplaceWidget(QWidget* parent = 0);
    ~FindReplaceWidget();

public slots:
    void setFocus();

private:
    Ui::FindReplaceWidget* mUi;
};

#endif // FINDREPLACEWIDGET_H
