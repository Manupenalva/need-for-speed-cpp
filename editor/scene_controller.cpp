#include "scene_controller.h"

#define GRID_SIZE 50

SceneController::SceneController(QGraphicsScene* scene): scene(scene) {}

void SceneController::handleDropEvent(const DragInfo& dragInfo, int x, int y, bool del) {
    if (del) {
        QList<QGraphicsItem*> itemsAtPos = scene->items(QPointF(x, y));
        for (QGraphicsItem* item: itemsAtPos) {
            // cppcheck-suppress useStlAlgorithm
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
    const auto& items = scene->items();
    int count = std::count_if(items.begin(), items.end(), [&type](QGraphicsItem* item) {
        return item->data(0).isValid() &&
               item->data(0).toString().contains(type, Qt::CaseInsensitive);
    });
    return count;
}
