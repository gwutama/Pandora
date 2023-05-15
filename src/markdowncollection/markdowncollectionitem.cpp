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


QString MarkdownCollectionItem::richTitle()
{
    QString richTitle("<h3>%1</h3><p>This is the content</p>");
    richTitle = richTitle.arg(title());
    return richTitle;
}
