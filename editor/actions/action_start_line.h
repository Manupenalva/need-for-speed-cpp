#ifndef ACTION_START_LINE_H
#define ACTION_START_LINE_H

#include "actions.h"

class ActionStartLine: public Actions {
public:
    ActionResult execute(SceneController& controller, const DragInfo& info, int x, int y) override;

    YAML::Node writeNode(const QGraphicsItem* item) const override;

    ItemRecord loadNode(const YAML::Node& node) const override;
};

#endif
