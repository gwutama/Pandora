#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QTemporaryFile>
#include "common/appconfig.h"
#include "preferences/preferencesdialog.h"
#include "markdowneditor/markdowneditor.h"
#include "markdownviewer/markdownviewer.h"
#include "markdowncollection/collectionlistview.h"
#include "markdowncollection/markdowncollection.h"
#include "maintoolbar.h"

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
    void onSaveAsActionTriggered(bool checked);
    void onExportAsActionTriggered(bool checked);
    void onTogglePreviewActionTriggered(bool checked);
    void onToggleCollectionItemsActionTriggered(bool checked);

    void onCollectionItemActivated(QSharedPointer<MarkdownCollectionItem> item);
    void onCollectionItemRemoved();

private:
    Ui::MainWindow* mUi;
    PreferencesDialog* mCfgDialog;
    MarkdownViewer* mViewer;
    MarkdownEditor* mEditor;
    MainToolbar* mToolbar;
    CollectionListView* mCollectionList;
    QSharedPointer<MarkdownCollection> mCollection;
    QSharedPointer<AppConfig> mConfig;
    QSharedPointer<QTemporaryFile> mTmpMarkdownFile; // for "new" file (not saved yet)

    static const char* sTag;
};

#endif // MAINWINDOW_H
