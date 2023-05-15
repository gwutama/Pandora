#include "documentstatisticsdialog.h"
#include "ui_documentstatisticsdialog.h"
#include <QTableWidgetItem>
#include <QDebug>

const char* DocumentStatisticsDialog::sTag = "[DocumentStatisticsDialog]";

DocumentStatisticsDialog::DocumentStatisticsDialog(QTextDocument* document,
                                                   QWidget* parent) :
    QDialog(parent),
    mUi(new Ui::DocumentStatisticsDialog),
    mDocument(document)
{
    mUi->setupUi(this);
    setupTable();
}


DocumentStatisticsDialog::~DocumentStatisticsDialog()
{
    delete mUi;
}


void DocumentStatisticsDialog::setupTable()
{
    mUi->tableWidget->setRowCount(3);
    mUi->tableWidget->setColumnCount(2);
    mUi->tableWidget->horizontalHeader()->hide();
    mUi->tableWidget->verticalHeader()->hide();
    mUi->tableWidget->setColumnWidth(0, 200);
    mUi->tableWidget->horizontalHeader()->setStretchLastSection(true);

    QTableWidgetItem* wordCount = new QTableWidgetItem("Word count");
    QTableWidgetItem* wordCountVal = new QTableWidgetItem();
    QTableWidgetItem* charCount = new QTableWidgetItem("Character count");
    QTableWidgetItem* charCountVal = new QTableWidgetItem();
    QTableWidgetItem* lineCount = new QTableWidgetItem("Line count");
    QTableWidgetItem* lineCountVal = new QTableWidgetItem();

    mUi->tableWidget->setItem(0, 0, charCount);
    mUi->tableWidget->setItem(0, 1, charCountVal); // char count: line 1
    mUi->tableWidget->setItem(1, 0, wordCount);
    mUi->tableWidget->setItem(1, 1, wordCountVal); // word count: line 2
    mUi->tableWidget->setItem(2, 0, lineCount);
    mUi->tableWidget->setItem(2, 1, lineCountVal); // line count: line 3
}


void DocumentStatisticsDialog::show()
{
    populate();
    QDialog::show();
}


void DocumentStatisticsDialog::populate()
{
    if (mDocument == nullptr)
    {
        qWarning() << sTag << "Document is null";
        return;
    }

    // char count: line 1
    // word count: line 2
    // line count: line 3
    mUi->tableWidget->item(0, 1)->setText(QString::number(mDocument->characterCount()));

    int numWords = mDocument->toPlainText().split(" ").size();
    mUi->tableWidget->item(1, 1)->setText(QString::number(numWords));

    mUi->tableWidget->item(2, 1)->setText(QString::number(mDocument->lineCount()));
}
