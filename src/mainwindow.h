#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QUrl>
#include <QSharedPointer>
#include "preferencesdialog.h"
#include "documentgenerator.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private slots:
    void generate();
    void onScrollPositionChanged(const QPointF pos);
    void onPageLoaded(bool ok);

private:
    void initDocumentGenerator();
    void loadPage(const QString& path);

    void closeEvent(QCloseEvent* event);

private:
    Ui::MainWindow* mUi;
    QWebEngineView* mWebView;
    PreferencesDialog* mCfgDialog;
    QSharedPointer<AppConfig> mConfig;
    QSharedPointer<DocumentGenerator> mGenerator;
    QPointF mScrollPos;
};

#endif // MAINWINDOW_H
