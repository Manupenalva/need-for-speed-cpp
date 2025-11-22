#include "action_start_line.h"

ActionResult ActionStartLine::execute(SceneController& controller, const DragInfo& info, int x, int y) {
    if (controller.countItemsOfType(LINE_TYPE) >= MAX_START_LINE) {
        return ActionResult{false,
                            false,
                            {},
                            QPointF{},
                            "Limit reach",
                            "There is a starting point in the map. Please remove to place a new one."};
    }
    controller.handleDropEvent(info, x, y);
    return ActionResult{true, false, {}, QPointF{}};
}

YAML::Node ActionStartLine::writeNode(const QGraphicsItem* item) const {
    YAML::Node node;
    QPointF pos = item->pos();
    node["x"] = static_cast<int>(pos.x());
    node["y"] = static_cast<int>(pos.y());
    int rotation = item->data(ROTATION).toInt();
    if (rotation == LINE_ROTATION) {
        node["rotation"] = "vertical";
    } else {
        node["rotation"] = "horizontal";
    }
    return node;
}

ItemRecord ActionStartLine::loadNode(const YAML::Node& node) const {
    int x = node["x"].as<int>();
    int y = node["y"].as<int>();
    int rotationDeg = 0;
    if (node["rotation"]) {
        QString rotationStr = QString::fromStdString(node["rotation"].as<std::string>());
        if (rotationStr == "vertical")
            rotationDeg = LINE_ROTATION;
    }
    return ItemRecord{DragInfo(LINE_TYPE, rotationDeg, QString{}, -1), QPoint(x, y)};
}
