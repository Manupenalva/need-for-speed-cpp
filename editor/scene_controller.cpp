#include "scene_controller.h"

#include <algorithm>

#include "editor_constants.h"

SceneController::SceneController(QGraphicsScene* scene): scene(scene) {}

void SceneController::handleDropEvent(const DragInfo& dragInfo, int x, int y) {
    QGraphicsPixmapItem* item = itemBuilder.buildItem(dragInfo, QSize(GRID_SIZE, GRID_SIZE));
    if (!item) {
        return;
    }

    item->setPos(x, y);
    if (dragInfo.getType().contains(CHECKPOINT_TYPE, Qt::CaseInsensitive)) {
        if (dragInfo.getId() > 0) {
            item->setData(ID, dragInfo.getId());
            id = std::max(id, dragInfo.getId());
        } else {
            ++id;
            item->setData(ID, id);
        }
    }
    if (dragInfo.getType().contains(HINT_TYPE, Qt::CaseInsensitive)) {
        if (dragInfo.getId() > 0) {
            item->setData(ID, dragInfo.getId());
        }
    }
    scene->addItem(item);
}

int SceneController::countItemsOfType(const QString& type) const {
    const auto all = scene->items();
    return static_cast<int>(std::count_if(all.cbegin(), all.cend(), [&](QGraphicsItem* item) {
        return item->data(TYPE).isValid() &&
               item->data(TYPE).toString().contains(type, Qt::CaseInsensitive);
    }));
}

void SceneController::placeHint(const DragInfo& info, const QPointF& hintPos,
                                QGraphicsItem* checkpointItem) {
    QGraphicsPixmapItem* hint = itemBuilder.buildItem(info, QSize(GRID_SIZE, GRID_SIZE));
    hint->setPos(hintPos.x(), hintPos.y());
    hint->setData(ID, checkpointItem->data(ID).toInt());
    scene->addItem(hint);
}

void SceneController::deleteHints(QGraphicsItem* checkpointItem) {
    int idCheckpoint = checkpointItem->data(ID).toInt();
    scene->removeItem(checkpointItem);
    delete checkpointItem;
    QList<QGraphicsItem*> hintToDelete;
    for (auto* it: scene->items()) {
        const auto type = it->data(TYPE).toString();
        if (type.contains(HINT_TYPE, Qt::CaseInsensitive) && it->data(ID).toInt() == idCheckpoint) {
            hintToDelete.append(it);
        }
    }

    for (auto* hint: hintToDelete) {
        deleteItem(hint);
    }
}

void SceneController::deleteItem(QGraphicsItem* item) {
    auto type = item->data(TYPE).toString();
    if (type.contains(CHECKPOINT_TYPE, Qt::CaseInsensitive)) {
        deleteHints(item);
        return;
    }
    scene->removeItem(item);
    delete item;
}
