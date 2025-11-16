#ifndef ACTION_START_H
#define ACTION_START_H

#include "actions.h"

class ActionStart : public Actions {
public:
    ActionResult execute(SceneController& controller, const DragInfo& info, int x, int y) override;
};

#endif