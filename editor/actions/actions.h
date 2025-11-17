#ifndef ACTIONS_H
#define ACTIONS_H

#include <QGraphicsItem>

#include <yaml-cpp/yaml.h>

#include "../editor_constants.h"
#include "../scene_controller.h"

#include "action_result.h"

struct ItemRecord {
    DragInfo info;
    QPoint pos;
};


class Actions {
public:
    virtual ~Actions() = default;

    virtual ActionResult execute(SceneController& controller, const DragInfo& info, int x,
                                 int y) = 0;

    virtual YAML::Node writeNode(const QGraphicsItem* item) const = 0;

    virtual ItemRecord loadNode(const YAML::Node& node) const = 0;
};

#endif
