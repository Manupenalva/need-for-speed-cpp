#include "scene_controller.h"

#include <QHash>
#include <QList>
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
        auto* label = new QGraphicsSimpleTextItem(QString::number(item->data(ID).toInt()), item);
        label->setPos(5, -5);
    }
    if (dragInfo.getType().contains(HINT_TYPE, Qt::CaseInsensitive)) {
        if (dragInfo.getId() > 0) {
            item->setData(ID, dragInfo.getId());
            auto* label = new QGraphicsSimpleTextItem(QString::number(dragInfo.getId()), item);
            label->setPos(5, -5);
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
    auto* label =
            new QGraphicsSimpleTextItem(QString::number(checkpointItem->data(ID).toInt()), hint);
    label->setPos(5, -5);
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
    renumberCheckpoints();
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

void SceneController::renumberCheckpoints() {
    QList<QGraphicsItem*> checkpoints;
    for (auto* item: scene->items()) {
        const auto type = item->data(TYPE).toString();
        if (type.contains(CHECKPOINT_TYPE, Qt::CaseInsensitive)) {
            checkpoints.append(item);
        }
    }

    if (checkpoints.isEmpty()) {
        id = 0;
        return;
    }
    std::sort(checkpoints.begin(), checkpoints.end(), [](QGraphicsItem* a, QGraphicsItem* b) {
        return a->data(ID).toInt() < b->data(ID).toInt();
    });

    QHash<int, int> newCheckpoints;
    int newId = 1;
    for (auto* cp: checkpoints) {
        int oldId = cp->data(ID).toInt();
        newCheckpoints.insert(oldId, newId);
        cp->setData(ID, newId);
        for (auto* child: cp->childItems()) {
            if (auto* text = dynamic_cast<QGraphicsSimpleTextItem*>(child)) {
                text->setText(QString::number(newId));
            }
        }
        ++newId;
    }

    id = newId - 1;

    for (auto* item: scene->items()) {
        const auto type = item->data(TYPE).toString();
        if (!type.contains(HINT_TYPE, Qt::CaseInsensitive)) {
            continue;
        }
        int oldId = item->data(ID).toInt();
        if (!newCheckpoints.contains(oldId)) {
            continue;
        }
        item->setData(ID, newCheckpoints.value(oldId));
        for (auto* child: item->childItems()) {
            if (auto* text = dynamic_cast<QGraphicsSimpleTextItem*>(child)) {
                text->setText(QString::number(newId));
            }
        }
    }
}
