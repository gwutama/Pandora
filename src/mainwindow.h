#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QTemporaryFile>
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
    void uiStateNoFileOpen();
    void uiStateFileOpened();
    void closeEvent(QCloseEvent* event);
    bool openFileHelper(const QString& file);
    void closeFileHelper();

private slots:
    void onNewActionTriggered(bool checked);
    void onOpenActionTriggered(bool checked);
    void onCloseActionTriggered(bool checked);
    void onSaveActionTriggered(bool checked);

private:
    Ui::MainWindow* mUi;
    PreferencesDialog* mCfgDialog;
    MarkdownViewer* mViewer;
    MarkdownEditor* mEditor;
    QSharedPointer<AppConfig> mConfig;
    QSharedPointer<QTemporaryFile> mTmpMarkdownFile; // for "new" file (not saved yet)

    static const char* sTag;
};

#endif // MAINWINDOW_H
