#ifndef ACTION_CHECKPOINT_H
#define ACTION_CHECKPOINT_H

#include "actions.h"

class ActionCheckpoint : public Actions {
public:
    ActionResult execute(SceneController& controller, const DragInfo& info, int x, int y) override;

    YAML::Node writeNode(const QGraphicsItem* item) const override;

    ItemRecord loadNode(const YAML::Node& node) const override;
};

#endif