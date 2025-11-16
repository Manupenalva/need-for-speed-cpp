#include "action_hint.h"

#include "../editor_constants.h"

ActionResult ActionHint::execute(SceneController& controller, const DragInfo& info, int x, int y) {
    if (controller.countItemsOfType(CHECKPOINT_TYPE) < 1) {
        return ActionResult{false, false, {}, QPointF{}, "No checkpoint", "You must place at least one checkpoint before placing hints"};
    }
    return ActionResult{true, true, info, QPointF(x, y)};
}