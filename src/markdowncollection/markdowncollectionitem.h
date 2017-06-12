#ifndef MARKDOWNCOLLECTIONITEM_H
#define MARKDOWNCOLLECTIONITEM_H

#include <QObject>

class MarkdownCollectionItem : public QObject
{
    Q_OBJECT
public:
    explicit MarkdownCollectionItem(QObject* parent = nullptr);
    virtual ~MarkdownCollectionItem();

    inline qint64 creationTimestamp()
    { return mCreationTimestamp; }

    inline qint64 modificationTimestamp()
    { return mModificationTimestamp; }

    void setContent(const QString& content);

    inline QString content()
    { return mContent; }

private:
    qint64 mCreationTimestamp;
    qint64 mModificationTimestamp;
    QString mContent;
};

#endif // MARKDOWNCOLLECTIONITEM_H
