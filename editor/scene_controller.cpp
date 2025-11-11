#include "scene_controller.h"

#include <algorithm>

#define GRID_SIZE 50

SceneController::SceneController(QGraphicsScene* scene): scene(scene) {}

void SceneController::handleDropEvent(const DragInfo& dragInfo, int x, int y) {
    QGraphicsPixmapItem* item = itemBuilder.buildItem(dragInfo, QSize(GRID_SIZE, GRID_SIZE));
    if (!item) {
        return;
    }

    item->setPos(x, y);
    if (dragInfo.getType().contains("checkpoint", Qt::CaseInsensitive)) {
        ++id;
        item->setData(2, id);
    }
    scene->addItem(item);
}

int SceneController::countItemsOfType(const QString& type) const {
    const auto all = scene->items();
    return static_cast<int>(std::count_if(all.cbegin(), all.cend(), [&](QGraphicsItem* item) {
        return item->data(0).isValid() &&
               item->data(0).toString().contains(type, Qt::CaseInsensitive);
    }));
}

void SceneController::countCheckpointsIds() {
    id = 0;
    for (auto* it: scene->items()) {
        const auto t = it->data(0).toString();
        if (t.contains("checkpoint", Qt::CaseInsensitive)) {
            id = std::max(id, it->data(2).toInt());
        }
    }
}
