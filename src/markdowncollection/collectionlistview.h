#ifndef COLLECTIONLISTVIEW_H
#define COLLECTIONLISTVIEW_H

#include <QListView>
#include <QStandardItemModel>
#include "markdowncollection.h"

class CollectionListView : public QListView
{
    Q_OBJECT

public:
    explicit CollectionListView(QWidget* parent = nullptr);
    virtual ~CollectionListView();

public slots:
    void newItem();

private slots:
    void showContextMenu(const QPoint& point);
    void deleteItems();
    void onItemRenamed(const QModelIndex& topLeft,
                       const QModelIndex& bottomRight,
                       const QVector<int>& roles = QVector<int>());

private:
    QStandardItemModel* mCollectionModel;
    MarkdownCollection mCollection;
    static const char* sTag;
};

#endif // COLLECTIONLISTVIEW_H
