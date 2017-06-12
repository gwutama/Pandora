#ifndef MARKDOWNCOLLECTION_H
#define MARKDOWNCOLLECTION_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include "markdowncollectionitem.h"

class MarkdownCollection : public QObject
{
    Q_OBJECT

public:
    explicit MarkdownCollection(QObject* parent = nullptr);
    virtual ~MarkdownCollection();

    inline QList< QSharedPointer<MarkdownCollectionItem> > items()
    { return mItems; }

    bool appendItem(QSharedPointer<MarkdownCollectionItem> item);
    bool removeOneItem(QSharedPointer<MarkdownCollectionItem> item);

    void insertItem(QSharedPointer<MarkdownCollectionItem> item, uint pos)
    { mItems.insert(pos, item); }

    QSharedPointer<MarkdownCollectionItem> takeOneItem(uint pos);

    inline void clearItems()
    { mItems.clear(); }

private:
    QList< QSharedPointer<MarkdownCollectionItem> > mItems;

    static const char* sTag;
};

#endif // MARKDOWNCOLLECTION_H
