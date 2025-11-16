#ifndef ACTION_ROAD_H
#define ACTION_ROAD_H

#include "actions.h"

class ActionRoad : public Actions {
public:
    ActionResult execute(SceneController& controller, const DragInfo& info, int x, int y) override;
};

#endif