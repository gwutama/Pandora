#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QUrl>
#include <QSharedPointer>
#include "appconfig.h"
#include "preferencesdialog.h"
#include "markdowneditor.h"
#include "markdownviewer.h"

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

private:
    void closeEvent(QCloseEvent* event);

private slots:
    void onOpenActionTriggered(bool checked);
    void onCloseActionTriggered(bool checked);
    void onSaveActionTriggered(bool checked);

private:
    Ui::MainWindow* mUi;    
    PreferencesDialog* mCfgDialog;
    MarkdownViewer* mViewer;
    MarkdownEditor* mEditor;
    QSharedPointer<AppConfig> mConfig;

    static const char* sTag;
};

#endif // MAINWINDOW_H
