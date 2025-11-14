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
        if (dragInfo.getId() > 0) {
            item->setData(2, dragInfo.getId());
            id = std::max(id, dragInfo.getId());
        } else {
            ++id;
            item->setData(2, id);
        }
    }
    if (dragInfo.getType().contains("hint", Qt::CaseInsensitive)) {
        if (dragInfo.getId() > 0) {
            item->setData(2, dragInfo.getId());
        } 
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

void SceneController::placeHint(const DragInfo& info, const QPointF& hintPos, QGraphicsItem* checkpointItem) {
    QGraphicsPixmapItem* hint = itemBuilder.buildItem(info, QSize(GRID_SIZE, GRID_SIZE));
    hint->setPos(hintPos.x(), hintPos.y());
    hint->setData(2, checkpointItem->data(2).toInt());
    scene->addItem(hint);
}

void SceneController::deleteHints(QGraphicsItem* checkpointItem) {
    int idCheckpoint = checkpointItem->data(2).toInt();
    scene->removeItem(checkpointItem);
    delete checkpointItem;
    QList<QGraphicsItem*> hintToDelete;
    for (auto* it: scene->items()) {
        const auto type = it->data(0).toString();
        if (type.contains("hint", Qt::CaseInsensitive) && it->data(2).toInt() == idCheckpoint) {
            hintToDelete.append(it);
        }
    }

    for (auto* hint: hintToDelete) {
        scene->removeItem(hint);
        delete hint;
    }
}