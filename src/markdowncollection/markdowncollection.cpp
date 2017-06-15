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


bool MarkdownCollection::removeItem(const QUuid& uid)
{
    if (mItems.contains(uid))
    {
        return mItems.remove(uid);
    }

    qWarning() << sTag << "Cannot remove item from collection because item does not exist";
    return false;
}


QStringList MarkdownCollection::uids()
{
    QList<QUuid> uids = mItems.keys();
    QStringList out;

    foreach (QUuid uid, uids)
    {
        out.append(uid.toString());
    }

    return out;
}
