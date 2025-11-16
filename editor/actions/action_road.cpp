#include "action_road.h"

ActionResult ActionRoad::execute(SceneController& controller, const DragInfo& info, int x, int y) {
    controller.handleDropEvent(info, x, y);
    return ActionResult{true, false, {}, QPointF{}};
}