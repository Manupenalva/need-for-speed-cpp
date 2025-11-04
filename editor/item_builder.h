#ifndef ITEM_BUILDER_H
#define ITEM_BUILDER_H

#include <QGraphicsPixmapItem>
#include <QSize>
#include <QString>

#include "drag_info.h"

class ItemBuilder {
public:
    ItemBuilder() = default;

    QGraphicsPixmapItem* buildItem(const DragInfo& info, const QSize& size);

private:
    QString getIcon(const QString& type) const;
};

#endif
