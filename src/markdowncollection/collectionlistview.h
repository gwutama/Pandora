#ifndef COLLECTIONLISTVIEW_H
#define COLLECTIONLISTVIEW_H

#include <QListView>
#include <QStandardItemModel>
#include "markdowncollection.h"
#include "richtextstyleditemdelegate.h"

class CollectionListView : public QListView
{
    Q_OBJECT

public:
    explicit CollectionListView(QSharedPointer<MarkdownCollection> collection,
                                QWidget* parent = nullptr);
    virtual ~CollectionListView();

public slots:
//    void updateItem();

private slots:    
    void showContextMenu(const QPoint& point);
    void createItem();
    void deleteSelectedItems();
    void onRowsRemoved(const QModelIndex& parent, int first, int last);
    void onRowsInserted(const QModelIndex& parent, int first, int last);
    void onItemSelected(const QModelIndex& index);

signals:
    void collectionItemActivated(QSharedPointer<MarkdownCollectionItem> item);
    void collectionItemRemoved();

private:
    QStandardItemModel* mCollectionModel;
    QSharedPointer<MarkdownCollection> mCollection;
    RichTextStyledItemDelegate* mRichTextDelegate;
    int mModelIndexMoved;
    QUuid mActiveCollectionItemUid;
    static const char* sTag;
};

#endif // COLLECTIONLISTVIEW_H
