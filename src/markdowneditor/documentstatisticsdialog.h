#ifndef DOCUMENTSTATISTICSDIALOG_H
#define DOCUMENTSTATISTICSDIALOG_H

#include <QDialog>
#include <QTextDocument>

namespace Ui
{
class DocumentStatisticsDialog;
}

class DocumentStatisticsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DocumentStatisticsDialog(QTextDocument* document,
                                      QWidget* parent = 0);
    ~DocumentStatisticsDialog();

public slots:
    void show();

private slots:
    void populate();

private:
    void setupTable();

private:
    Ui::DocumentStatisticsDialog* mUi;
    QTextDocument* mDocument;
    static const char* sTag;
};

#endif // DOCUMENTSTATISTICSDIALOG_H
