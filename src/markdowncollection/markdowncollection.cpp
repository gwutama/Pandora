#include "markdowncollection.h"
#include <QDebug>

const char* MarkdownCollection::sTag = "[MarkdownCollection]";

MarkdownCollection::MarkdownCollection(QObject* parent) :
    QObject(parent)
{
}


MarkdownCollection::~MarkdownCollection()
{
}


bool MarkdownCollection::appendItem(QSharedPointer<MarkdownCollectionItem> item)
{
    if (!mItems.contains(item))
    {
        mItems.append(item);
        return true;
    }

    qWarning() << sTag << "Cannot append item into collection because item exists";
    return false;
}


bool MarkdownCollection::removeOneItem(QSharedPointer<MarkdownCollectionItem> item)
{
    if (mItems.contains(item))
    {
        return mItems.removeOne(item);
    }

    qWarning() << sTag << "Cannot remove item from collection because item does not exist";
    return false;
}


QSharedPointer<MarkdownCollectionItem> MarkdownCollection::takeOneItem(uint pos)
{
    if (pos < mItems.size())
    {
        return mItems.takeAt(pos);
    }

    qWarning() << sTag << "Index out of bound";
    return QSharedPointer<MarkdownCollectionItem>();
}
