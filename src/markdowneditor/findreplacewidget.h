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

    void setNumMatches(int numMatches);

    inline int numMatches()
    { return mNumMatches; }

    QString searchTerm();
    QString replacementText();

public slots:
    void setFocus();

signals:
    void wantToExecuteSearch();
    void nextButtonClicked();
    void previousButtonClicked();
    void replaceButtonClicked();
    void replaceFindButtonClicked();
    void replaceAllButtonClicked();

private:
    Ui::FindReplaceWidget* mUi;
    int mNumMatches;
};

#endif // FINDREPLACEWIDGET_H
