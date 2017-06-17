#include "collectionlistview.h"
#include <QMenu>
#include <QDebug>

const char* CollectionListView::sTag = "[CollectionListView]";

CollectionListView::CollectionListView(QWidget* parent) :
    QListView(parent)
{
    setFixedWidth(250);
    setStyleSheet("QListView { border: none }");
    setContextMenuPolicy(Qt::CustomContextMenu);

    mCollectionModel = new QStandardItemModel(this);
    setModel(mCollectionModel);

    mRichTextDelegate = new RichTextStyledItemDelegate;
    setItemDelegate(mRichTextDelegate);

    // Signals slots
    connect(this, &CollectionListView::customContextMenuRequested,
            this, &CollectionListView::showContextMenu);
    connect(mCollectionModel, &QStandardItemModel::dataChanged,
            this, &CollectionListView::onItemRenamed);
}


CollectionListView::~CollectionListView()
{
    if (mRichTextDelegate)
    {
        delete (mRichTextDelegate);
        mRichTextDelegate = 0;
    }
}


void CollectionListView::showContextMenu(const QPoint& point)
{
    QMenu menu;
    menu.addAction("New Item", this, SLOT(newItem()));

    if (!selectedIndexes().isEmpty())
    {
        menu.addAction("Delete Item", this, SLOT(deleteItems()));
    }

    menu.exec(mapToGlobal(point));
}


void CollectionListView::newItem()
{
    MarkdownCollectionItem* itemPtr = new MarkdownCollectionItem;
    itemPtr->mTitle = "Item Title";
    auto item = QSharedPointer<MarkdownCollectionItem>(itemPtr);
    mCollection.insertItem(item);

    // Append to the end of the list
    QStandardItem* stdItem = new QStandardItem;
    stdItem->setData(item->uid(), Qt::UserRole);
    stdItem->setData(item->richTitle(), Qt::DisplayRole);
    mCollectionModel->appendRow(stdItem);

    qDebug() << sTag << "Created new item" << item->title() << item->uid();

    // Select the newly created item
    QModelIndex lastIdx = mCollectionModel->index(mCollectionModel->rowCount() - 1, 0);
    setCurrentIndex(lastIdx);
}


void CollectionListView::deleteItems()
{
    QModelIndexList modelIndexes = selectedIndexes();

    for (int i = 0; i < modelIndexes.size(); i++)
    {
        QModelIndex idx = modelIndexes.at(i);

        QStandardItem* stdItem = mCollectionModel->itemFromIndex(idx);
        QString title = stdItem->data(Qt::DisplayRole).toString();
        QUuid uid = stdItem->data(Qt::UserRole).toUuid();
        mCollectionModel->removeRow(idx.row()); // this also frees memory used by stdItem

        qDebug() << sTag << "Deleted selected item" << title << uid << "row" << idx.row();
    }
}


void CollectionListView::onItemRenamed(const QModelIndex& topLeft,
                                       const QModelIndex& /*bottomRight*/,
                                       const QVector<int>& /*roles*/)
{
    QString title =  topLeft.data().toString();

    QStandardItem* stdItem = mCollectionModel->itemFromIndex(topLeft);
    QUuid uid = stdItem->data(Qt::UserRole).toUuid();
    QSharedPointer<MarkdownCollectionItem> item = mCollection.findItem(uid);

    if (!item.isNull())
    {
        item->mTitle = title;
        qDebug() << sTag << "Item title renamed to" << topLeft.data();
    }
    else
    {
        qWarning() << sTag << "Cannot rename internal data: item is null";
    }
}
