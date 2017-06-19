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
    explicit CollectionListView(QWidget* parent = nullptr);
    virtual ~CollectionListView();

public slots:
    void newItem();

private slots:
    void showContextMenu(const QPoint& point);
    void deleteItems();
    void onRowsRemoved(const QModelIndex& parent, int first, int last);
    void onRowsInserted(const QModelIndex& parent, int first, int last);

private:
    QStandardItemModel* mCollectionModel;
    MarkdownCollection mCollection;
    RichTextStyledItemDelegate* mRichTextDelegate;
    int mModelIndexMoved;
    static const char* sTag;
};

#endif // COLLECTIONLISTVIEW_H
