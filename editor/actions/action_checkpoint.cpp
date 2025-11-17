#include "action_checkpoint.h"

ActionResult ActionCheckpoint::execute(SceneController& controller, const DragInfo& info, int x, int y) {
    controller.handleDropEvent(info, x, y);
    return ActionResult{true, false, {}, QPointF{}};
}

YAML::Node ActionCheckpoint::writeNode(const QGraphicsItem* item) const {
    YAML::Node node;
    QPointF pos = item->pos();
    node["x"] = static_cast<int>(pos.x());
    node["y"] = static_cast<int>(pos.y());
    int rotation = item->data(ROTATION).toInt();
    if (rotation == VERTICAL_ROTATION) {
        node["rotation"] = "vertical";
    } else {
        node["rotation"] = "horizontal";
    }
    node["id"] = item->data(ID).toInt();
    return node;
}

ItemRecord ActionCheckpoint::loadNode(const YAML::Node& node) const {
    int x = node["x"].as<int>();
    int y = node["y"].as<int>();
    int rotationDeg = 0;
    int id = -1;

    if (node["rotation"]) {
        QString rotationStr = QString::fromStdString(node["rotation"].as<std::string>());
        if (rotationStr == "vertical")
            rotationDeg = VERTICAL_ROTATION;
        else
            rotationDeg = HORIZONTAL_ROTATION;
    }
    id = node["id"].as<int>();
    return ItemRecord{DragInfo(CHECKPOINT_TYPE, rotationDeg, QString{}, id), QPoint(x, y)};
}