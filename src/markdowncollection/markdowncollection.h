#ifndef MARKDOWNCOLLECTION_H
#define MARKDOWNCOLLECTION_H

#include <QObject>
#include <QMap>
#include <QUuid>
#include <QSharedPointer>
#include "markdowncollectionitem.h"

class MarkdownCollection : public QObject
{
    Q_OBJECT

public:
    explicit MarkdownCollection(QObject* parent = nullptr);
    virtual ~MarkdownCollection();

    inline QMap< QUuid, QSharedPointer<MarkdownCollectionItem> > items()
    { return mItems; }

    bool removeItem(const QUuid& uid);

    void insertItem(QSharedPointer<MarkdownCollectionItem> item)
    { mItems.insert(item->uid(), item); }

    inline QSharedPointer<MarkdownCollectionItem> findItem(const QUuid& uid)
    { return mItems.value(uid); }

    inline void clearItems()
    { mItems.clear(); }

    QStringList uids();

private:
    QMap< QUuid, QSharedPointer<MarkdownCollectionItem> > mItems;

    static const char* sTag;
};

#endif // MARKDOWNCOLLECTION_H
