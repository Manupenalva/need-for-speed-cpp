#ifndef ACTION_FINISH_H
#define ACTION_FINISH_H

#include "actions.h"

class ActionFinish : public Actions {
public:
    ActionResult execute(SceneController& controller, const DragInfo& info, int x, int y) override;
};

#endif