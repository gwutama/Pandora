#ifndef RICHTEXTSTYLEDITEMDELEGATE_H
#define RICHTEXTSTYLEDITEMDELEGATE_H

#include <QStyledItemDelegate>

/**
 * @brief The RichTextStyledItemDelegate class
 * Resource:
 * https://stackoverflow.com/questions/1956542/how-to-make-item-view-render-rich-html-text-in-qt
 */
class RichTextStyledItemDelegate : public QStyledItemDelegate
{
protected:
    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const;

    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const;
};

#endif // RICHTEXTSTYLEDITEMDELEGATE_H
