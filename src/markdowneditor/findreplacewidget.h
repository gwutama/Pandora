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

    void setFoundNumber(int found);

public slots:
    void setFocus();

private slots:
    void onReturnPressed();

signals:
    void textChanged(const QString& text);

private:
    Ui::FindReplaceWidget* mUi;
};

#endif // FINDREPLACEWIDGET_H
