#include "item_builder.h"

#include <QPixmap>
#include <QTransform>

#include "editor_constants.h"

QGraphicsPixmapItem* ItemBuilder::buildItem(const DragInfo& info, const QSize& size) {
    QString iconPath;
    if (info.getIconPath().isEmpty()) {
        iconPath = getIcon(info.getType());
    } else {
        iconPath = info.getIconPath();
    }

    QPixmap pixmap(iconPath);
    if (pixmap.isNull()) {
        return nullptr;
    }

    if (info.getRotation() != 0) {
        QTransform transform;
        transform.rotate(info.getRotation());
        pixmap = pixmap.transformed(transform, Qt::SmoothTransformation);
    }

    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(
            pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    item->setData(TYPE, info.getType());
    item->setData(ROTATION, info.getRotation());

    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemSendsGeometryChanges);
    return item;
}

QString ItemBuilder::getIcon(const QString& type) const {
    if (type.contains(ROAD_TYPE, Qt::CaseInsensitive)) {
        return ROAD_PATH_1;
    } else if (type.contains(CHECKPOINT_TYPE, Qt::CaseInsensitive)) {
        return CHECKPOINT_PATH;
    } else if (type.contains(START_TYPE, Qt::CaseInsensitive)) {
        return START_PATH_1;
    } else if (type.contains(FINISH_TYPE, Qt::CaseInsensitive)) {
        return FINISH_PATH;
    } else if (type.contains(HINT_TYPE, Qt::CaseInsensitive)) {
        return HINT_PATH;
    } else {
        return "";
    }
}
