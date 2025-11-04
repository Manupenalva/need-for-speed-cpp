#include "drag_info.h"

#include <QJsonDocument>
#include <QJsonObject>

DragInfo::DragInfo(const QString& type, int rotation, const QString& iconPath):
        type(type), rotation(rotation), iconPath(iconPath) {}

QByteArray DragInfo::pack() const {
    QJsonObject jsonData;
    jsonData["type"] = type;
    jsonData["rotation"] = rotation;
    jsonData["iconPath"] = iconPath;

    QJsonDocument jsonDoc(jsonData);
    return jsonDoc.toJson(QJsonDocument::Compact);
}

bool DragInfo::unpack(const QByteArray& data) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (jsonDoc.isNull() || !jsonDoc.isObject())
        return false;

    QJsonObject jsonData = jsonDoc.object();
    type = jsonData.value("type").toString();
    rotation = jsonData.value("rotation").toInt();
    iconPath = jsonData.value("iconPath").toString();
    return true;
}

QString DragInfo::mimeType() const { return "application/x-maptool"; }

QString DragInfo::getType() const { return type; }

int DragInfo::getRotation() const { return rotation; }

QString DragInfo::getIconPath() const { return iconPath; }
