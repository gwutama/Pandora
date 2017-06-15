#include "markdowncollectionitem.h"
#include <QDateTime>

MarkdownCollectionItem::MarkdownCollectionItem(QObject* parent) :
    QObject(parent)
{
    mCreationTimestamp = QDateTime::currentMSecsSinceEpoch();
    mUid = QUuid::createUuid();
}


MarkdownCollectionItem::~MarkdownCollectionItem()
{
}


void MarkdownCollectionItem::setContent(const QString& content)
{
    mContent = content;
    mModificationTimestamp = QDateTime::currentMSecsSinceEpoch();
}
