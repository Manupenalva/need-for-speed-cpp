#ifndef ACTIONS_H
#define ACTIONS_H

#include "action_result.h"
#include "../scene_controller.h"

struct ItemRecord
{
    DragInfo info;
    QPoint pos;
};


class Actions {
public:
    virtual ~Actions() = default;

    virtual ActionResult execute(SceneController& controller, const DragInfo& info, int x, int y) = 0;
};

#endif