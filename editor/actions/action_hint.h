#ifndef ACTION_HINT_H
#define ACTION_HINT_H

#include "actions.h"

class ActionHint : public Actions {
public:
    ActionResult execute(SceneController& controller, const DragInfo& info, int x, int y) override;
};

#endif