#ifndef SCENE_CONTROLLER_H
#define SCENE_CONTROLLER_H

#include <QGraphicsScene>
#include <QString>
#include <QGraphicsItem>
#include "drag_info.h"
#include "item_builder.h"

class SceneController {
public:
    SceneController(QGraphicsScene* scene);

    void handleDropEvent(const DragInfo& dragInfo, int x, int y, bool del);
    int countItemsOfType(const QString& type) const;

private:
    QGraphicsScene* scene;
    ItemBuilder itemBuilder;
};

#endif