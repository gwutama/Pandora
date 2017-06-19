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

    // Drag and drop model.
    // Resources:
    // http://www.qtforum.org/article/12733/how-to-make-drag-drop-in-qlistview.html
    // https://stackoverflow.com/questions/40165709/moving-items-via-dragdrop-in-qlistview-iconmode-removes-them-incorrectly-w
    setDragEnabled(true);
    setDragDropMode(QListView::InternalMove);
    setDefaultDropAction(Qt::MoveAction);

    mCollectionModel = new QStandardItemModel(this);
    setModel(mCollectionModel);

    mRichTextDelegate = new RichTextStyledItemDelegate;
    setItemDelegate(mRichTextDelegate);

    // Signals slots
    connect(this, &CollectionListView::customContextMenuRequested,
            this, &CollectionListView::showContextMenu);
    connect(mCollectionModel, &QStandardItemModel::rowsRemoved,
            this, &CollectionListView::onRowsRemoved);
    connect(mCollectionModel, &QStandardItemModel::rowsInserted,
            this, &CollectionListView::onRowsInserted);
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
    itemPtr->mTitle = "Item Title " + QString::number(qrand() % 100);
    auto item = QSharedPointer<MarkdownCollectionItem>(itemPtr);
    mCollection.insertItem(item);

    // Append to the end of the list
    QStandardItem* stdItem = new QStandardItem;
    stdItem->setData(item->uid(), Qt::UserRole);
    stdItem->setData(item->richTitle(), Qt::DisplayRole);
    stdItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
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


void CollectionListView::onRowsRemoved(const QModelIndex& parent, int first, int last)
{
    qDebug() << sTag << "Rows removed" << first << last;

    // If mModelIndexMoved is not -1, then a copy of an index was inserted, the old one
    // will be deleted here. What we want to do is select the copied index's item.
    if (mModelIndexMoved > -1)
    {
        bool isMovingUp = mModelIndexMoved < first; // seriously I don't understand why and how
        int pos = 0;

        // Again, not so sure but this apparently works.
        if (isMovingUp)
        {
            pos = mModelIndexMoved;
        }
        else
        {
            pos = mModelIndexMoved - 1;
        }

        QModelIndex idx = mCollectionModel->index(pos, 0);
        setCurrentIndex(idx);
        mModelIndexMoved = -1; // reset
    }
}


void CollectionListView::onRowsInserted(const QModelIndex& parent, int first, int last)
{
    qDebug() << sTag << "Rows inserted" << first << last;

    // On move with drag and drop, a copy of index is inserted first, then the old one will
    // be removed. See onRowsRemoved as well.
    // References:
    // https://forum.qt.io/topic/38540/keeping-selection-consistent-after-internalmove-in-qlistview/2
    mModelIndexMoved = first;
}
