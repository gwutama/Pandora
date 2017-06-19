#ifndef MARKDOWNCOLLECTIONITEM_H
#define MARKDOWNCOLLECTIONITEM_H

#include <QObject>
#include <QUuid>

class MarkdownCollectionItem : public QObject
{
    Q_OBJECT

    friend class MarkdownCollection;
    friend class CollectionListView;

public:
    explicit MarkdownCollectionItem(QObject* parent = nullptr);
    virtual ~MarkdownCollectionItem();

    inline QUuid uid()
    { return mUid; }

    inline qint64 creationTimestamp()
    { return mCreationTimestamp; }

    inline qint64 modificationTimestamp()
    { return mModificationTimestamp; }

    inline QString title()
    { return mTitle; }

    inline QString content()
    { return mContent; }

    QString richTitle();

private:
    void setContent(const QString& content);

private:
    QUuid mUid;
    qint64 mCreationTimestamp;
    qint64 mModificationTimestamp;
    QString mTitle;
    QString mContent;    
};

#endif // MARKDOWNCOLLECTIONITEM_H
