#include "action_checkpoint.h"

ActionResult ActionCheckpoint::execute(SceneController& controller, const DragInfo& info, int x, int y) {
    controller.handleDropEvent(info, x, y);
    return ActionResult{true, false, {}, QPointF{}};
}
