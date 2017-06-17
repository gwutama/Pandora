#include "richtextstyleditemdelegate.h"
#include <QPainter>
#include <QTextDocument>
#include <QApplication>
#include <QAbstractTextDocumentLayout>
#include <QDebug>

void RichTextStyledItemDelegate::paint(QPainter* painter,
                                       const QStyleOptionViewItem& option,
                                       const QModelIndex& index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QStyle* style = opt.widget ? opt.widget->style() : QApplication::style();

    QTextDocument doc;
    doc.setHtml(opt.text);
    doc.setTextWidth(opt.rect.width());
    doc.setDefaultFont(opt.font);

    // Painting item without text
    opt.text = QString();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

    QAbstractTextDocumentLayout::PaintContext ctx;

    // Highlighting text if item is selected
    if (opt.state & QStyle::State_Selected)
    {
        ctx.palette.setColor(QPalette::Text, opt.palette.color(QPalette::Active,
                                                               QPalette::HighlightedText));
    }
    else
    {
        ctx.palette.setColor(QPalette::Text, opt.palette.color(QPalette::Active, QPalette::Text));
    }

    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &opt);
    painter->save();
    painter->translate(textRect.topLeft());
    painter->setClipRect(textRect.translated(-textRect.topLeft()));
    doc.documentLayout()->draw(painter, ctx);
    painter->restore();
}


QSize RichTextStyledItemDelegate::sizeHint(const QStyleOptionViewItem& option,
                                           const QModelIndex& index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QTextDocument doc;
    doc.setHtml(opt.text);
    doc.setTextWidth(opt.rect.width());
    doc.setDefaultFont(opt.font);

    //    return QSize(doc.idealWidth(), doc.size().height());
    return QSize(doc.idealWidth(), 50); // seems to work
}
