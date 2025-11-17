#include "action_road.h"

ActionResult ActionRoad::execute(SceneController& controller, const DragInfo& info, int x, int y) {
    controller.handleDropEvent(info, x, y);
    return ActionResult{true, false, {}, QPointF{}};
}

YAML::Node ActionRoad::writeNode(const QGraphicsItem* item) const {
    YAML::Node node;
    QPointF pos = item->pos();
    node["x"] = static_cast<int>(pos.x());
    node["y"] = static_cast<int>(pos.y());
    return node;
}

ItemRecord ActionRoad::loadNode(const YAML::Node& node) const {
    int x = node["x"].as<int>();
    int y = node["y"].as<int>();
    return ItemRecord{DragInfo(ROAD_TYPE, 0, QString{}, -1), QPoint(x, y)};
}