#include "action_start.h"

#include "../editor_constants.h"

ActionResult ActionStart::execute(SceneController& controller, const DragInfo& info, int x, int y) {
    if (controller.countItemsOfType(START_TYPE) >= MAX_PLAYERS) {
        return ActionResult{false, false, {}, QPointF{}, "Limit reach", "There are 8 starting points in the map. Please remove one."};
    }
    controller.handleDropEvent(info, x, y);
    return ActionResult{true, false, {}, QPointF{}};
}   