#include "action_hint.h"

#include <string>

ActionResult ActionHint::execute(SceneController& controller, const DragInfo& info, int x, int y) {
    if (controller.countItemsOfType(CHECKPOINT_TYPE) < 1) {
        return ActionResult{false,
                            false,
                            {},
                            QPointF{},
                            "No checkpoint",
                            "You must place at least one checkpoint before placing hints"};
    }
    return ActionResult{true, true, info, QPointF(x, y)};
}

YAML::Node ActionHint::writeNode(const QGraphicsItem* item) const {
    YAML::Node node;
    QPointF pos = item->pos();
    node["x"] = static_cast<int>(pos.x());
    node["y"] = static_cast<int>(pos.y());
    int rotation = item->data(ROTATION).toInt();
    if (rotation == LEFT_ROTATION) {
        node["rotation"] = "left";
    } else if (rotation == RIGHT_ROTATION) {
        node["rotation"] = "right";
    } else if (rotation == UP_ROTATION) {
        node["rotation"] = "up";
    } else {
        node["rotation"] = "down";
    }
    node["id"] = item->data(ID).toInt();
    return node;
}

ItemRecord ActionHint::loadNode(const YAML::Node& node) const {
    int x = node["x"].as<int>();
    int y = node["y"].as<int>();
    int rotationDeg = 0;
    int id = -1;

    QString rotationStr = QString::fromStdString(node["rotation"].as<std::string>());
    if (rotationStr == "left")
        rotationDeg = LEFT_ROTATION;
    else if (rotationStr == "right")
        rotationDeg = RIGHT_ROTATION;
    else if (rotationStr == "up")
        rotationDeg = UP_ROTATION;
    else
        rotationDeg = DOWN_ROTATION;
    id = node["id"].as<int>();
    return ItemRecord{DragInfo(HINT_TYPE, rotationDeg, QString{}, id), QPoint(x, y)};
}
