#include "action_finish.h"

#include "../editor_constants.h"

ActionResult ActionFinish::execute(SceneController& controller, const DragInfo& info, int x, int y) {
    if (controller.countItemsOfType(FINISH_TYPE) >= MAX_FINISH) {
        return ActionResult{false, false, {}, QPointF{}, "Limit reach", "There is a finish line in the map. Please remove to drop the new one."};
    }
    controller.handleDropEvent(info, x, y);
    return ActionResult{true, false, {}, QPointF{}};
}