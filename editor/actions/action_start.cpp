#include "action_start.h"

ActionResult ActionStart::execute(SceneController& controller, const DragInfo& info, int x, int y) {
    if (controller.countItemsOfType(START_TYPE) >= MAX_PLAYERS) {
        return ActionResult{false, false, {}, QPointF{}, "Limit reach", "There are 8 starting points in the map. Please remove one."};
    }
    controller.handleDropEvent(info, x, y);
    return ActionResult{true, false, {}, QPointF{}};
}   

YAML::Node ActionStart::writeNode(const QGraphicsItem* item) const {
    YAML::Node node;
    QPointF pos = item->pos();
    node["x"] = static_cast<int>(pos.x());
    node["y"] = static_cast<int>(pos.y());
    int rotation = item->data(ROTATION).toInt();
    if (rotation == START_LEFT) {
        node["rotation"] = "left";
    } else if (rotation == START_RIGHT) {
        node["rotation"] = "right";
    } else if (rotation == START_UP) {
        node["rotation"] = "up";
    } else {
        node["rotation"] = "down";
    }
    return node;
}

ItemRecord ActionStart::loadNode(const YAML::Node& node) const {
    int x = node["x"].as<int>();
    int y = node["y"].as<int>();
    int rotationDeg = 0;
    int id = -1;

    QString rotationStr = QString::fromStdString(node["rotation"].as<std::string>());
    if (rotationStr == "left")
        rotationDeg = START_LEFT;
    else if (rotationStr == "right")
        rotationDeg = START_RIGHT;
    else if (rotationStr == "up")
        rotationDeg = START_UP;
    else
        rotationDeg = START_DOWN;
    return ItemRecord{DragInfo(START_TYPE, rotationDeg, QString{}, id), QPoint(x, y)};
}
