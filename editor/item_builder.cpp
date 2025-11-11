#include "item_builder.h"

#include <QTransform>
#include <QPixmap>

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
    item->setData(0, info.getType());
    item->setData(1, info.getRotation());

    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemSendsGeometryChanges);
    return item;
}

QString ItemBuilder::getIcon(const QString& type) const {
    if (type.contains("road", Qt::CaseInsensitive)) {
        return "../editor/imgs/road.png";
    } else if (type.contains("checkpoint", Qt::CaseInsensitive)) {
        return "../editor/imgs/checkpoint.png";
    } else if (type.contains("start", Qt::CaseInsensitive)) {
        return "../editor/imgs/start.png";
    } else if (type.contains("finish", Qt::CaseInsensitive)) {
        return "../editor/imgs/finish.png";
    } else if (type.contains("hint", Qt::CaseInsensitive)) {
        return "../editor/imgs/hint.png";
    } else if (type.contains("NPC", Qt::CaseInsensitive)) {
        return "../editor/imgs/npc.png";
    } else {
        return "";
    }
}

