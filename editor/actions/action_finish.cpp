#include "action_finish.h"

#include "../editor_constants.h"

ActionResult ActionFinish::execute(SceneController& controller, const DragInfo& info, int x, int y) {
    if (controller.countItemsOfType(FINISH_TYPE) >= MAX_FINISH) {
        return ActionResult{false, false, {}, QPointF{}, "Limit reach", "There is a finish line in the map. Please remove to drop the new one."};
    }
    controller.handleDropEvent(info, x, y);
    return ActionResult{true, false, {}, QPointF{}};
}

YAML::Node ActionFinish::writeNode(const QGraphicsItem* item) const {
    YAML::Node node;
    QPointF pos = item->pos();
    node["x"] = static_cast<int>(pos.x());
    node["y"] = static_cast<int>(pos.y());
    return node;
}

ItemRecord ActionFinish::loadNode(const YAML::Node& node) const {
    int x = node["x"].as<int>();
    int y = node["y"].as<int>();
    int rotationDeg = 0;
    int id = -1;
    return ItemRecord{DragInfo(FINISH_TYPE, rotationDeg, QString{}, id), QPoint(x, y)};
}
