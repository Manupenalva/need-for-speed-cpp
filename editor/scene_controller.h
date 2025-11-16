#ifndef SCENE_CONTROLLER_H
#define SCENE_CONTROLLER_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QString>

#include "drag_info.h"
#include "item_builder.h"

class SceneController {
public:
    explicit SceneController(QGraphicsScene* scene);

    void handleDropEvent(const DragInfo& dragInfo, int x, int y);
    int countItemsOfType(const QString& type) const;

    void countCheckpointsIds();

    void placeHint(const DragInfo& info, const QPointF& hintPos, QGraphicsItem* checkpointItem);
    void deleteHints(QGraphicsItem* checkpointItem);

    bool checkStartLine() const;
    bool checkFinishLine() const;

private:
    QGraphicsScene* scene;
    ItemBuilder itemBuilder;
    int id = 0;
};

#endif
