#include "scene_controller.h"

#define GRID_SIZE 50

SceneController::SceneController(QGraphicsScene* scene) : scene(scene) {}

void SceneController::handleDropEvent(const DragInfo& dragInfo, int x, int y, bool del) {
    if (del) {
        QList<QGraphicsItem*> itemsAtPos = scene->items(QPointF(x, y));
        for (QGraphicsItem* item : itemsAtPos) {
            if (item->data(0).isValid() && item->data(0).toString() == dragInfo.getType()) {
                scene->removeItem(item);
                delete item;
                return;
            }
        }
        return;
    }

    QGraphicsPixmapItem* item = itemBuilder.buildItem(dragInfo, QSize(GRID_SIZE, GRID_SIZE));
    if (!item) {
        return;
    }

    item->setPos(x, y);
    scene->addItem(item);
}

int SceneController::countItemsOfType(const QString& type) const {
    int count = 0;
    for (QGraphicsItem* item : scene->items()) {
        if (item->data(0).isValid() && item->data(0).toString() == type) {
            count++;
        }
    }
    return count;
}

