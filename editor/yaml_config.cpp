#include "yaml_config.h"

#include <QFile>
#include <QGraphicsItem>
#include <string>
#include <vector>

bool YamlConfig::save(const QGraphicsScene* scene, const QString& city, int gridSize,
                      const QString& path) {
    try {
        YAML::Emitter out;
        out << YAML::BeginMap;

        out << YAML::Key << "city" << YAML::Value << city.toStdString();
        out << YAML::Key << "width" << YAML::Value << static_cast<int>(scene->width());
        out << YAML::Key << "height" << YAML::Value << static_cast<int>(scene->height());
        out << YAML::Key << "celdWidth" << YAML::Value << gridSize;
        out << YAML::Key << "celdHeight" << YAML::Value << gridSize;

        writeElements(out, scene, "start");
        writeElements(out, scene, "finish");
        writeElements(out, scene, "road");
        writeElements(out, scene, "checkpoint");
        writeElements(out, scene, "hint");

        out << YAML::EndMap;

        QFile yamlFile(path);
        if (yamlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream yamlOut(&yamlFile);
            yamlOut << out.c_str();
            yamlFile.close();
        }
        return true;
    } catch (const YAML::Exception& e) {
        qWarning("Error al generar el YAML: %s", e.what());
        return false;
    }
}

void YamlConfig::writeElements(YAML::Emitter& out, const QGraphicsScene* scene,
                               const QString& elementType) {
    std::vector<YAML::Node> elements;
    for (QGraphicsItem* item: scene->items(Qt::AscendingOrder)) {
        if (!item->data(0).isValid())
            continue;
        QString type = item->data(0).toString();
        if (!type.contains(elementType, Qt::CaseInsensitive))
            continue;
        YAML::Node element;
        QPointF pos = item->pos();
        element["x"] = static_cast<int>(pos.x());
        element["y"] = static_cast<int>(pos.y());
        if (elementType == "hint") {
            int rotation = item->data(1).toInt();
            if (rotation == 0) {
                element["rotation"] = "left";
            } else if (rotation == 180) {
                element["rotation"] = "right";
            } else if (rotation == 90) {
                element["rotation"] = "up";
            } else {
                element["rotation"] = "down";
            }
            element["id"] = item->data(2).toInt();
        }
        if (elementType == "checkpoint") {
            element["id"] = item->data(2).toInt();
        }
        elements.push_back(element);
    }

    if (!elements.empty()) {
        out << YAML::Key << elementType.toStdString() << YAML::Value << YAML::BeginSeq;
        for (const auto& elem: elements) {
            out << elem;
        }
        out << YAML::EndSeq;
    }
}

bool YamlConfig::load(const QString& path) {
    try {
        QFile yamlFile(path);
        if (!yamlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning("No se pudo abrir el archivo YAML: %s", path.toStdString().c_str());
            return false;
        }

        QByteArray fileData = yamlFile.readAll();
        yamlFile.close();

        YAML::Node config = YAML::Load(fileData.constData());

        selectedCity = QString::fromStdString(config["city"].as<std::string>());
        items.clear();
        addElements(config, "start");
        addElements(config, "finish");
        addElements(config, "road");
        addElements(config, "checkpoint");
        addElements(config, "hint");
        return true;
    } catch (const YAML::Exception& e) {
        qWarning("Error al leer el YAML: %s", e.what());
        return false;
    }
}

void YamlConfig::addElements(const YAML::Node& config, const QString& elementType) {
    if (config[elementType.toStdString()]) {
        for (const auto& elem: config[elementType.toStdString()]) {
            int x = elem["x"].as<int>();
            int y = elem["y"].as<int>();
            int rotationDeg = 0;
            int id = -1;
            if (elementType == "hint" && elem["rotation"]) {
                QString rotationStr = QString::fromStdString(elem["rotation"].as<std::string>());
                if (rotationStr == "left")
                    rotationDeg = 0;
                else if (rotationStr == "right")
                    rotationDeg = 180;
                else if (rotationStr == "up")
                    rotationDeg = 90;
                else
                    rotationDeg = 270;
            }
            if (elementType == "checkpoint" || elementType == "hint") {
                id = elem["id"].as<int>();
            }
            items.emplace_back(DragInfo(elementType, rotationDeg, QString{}, id), QPoint(x, y));
        }
    }
}

QString YamlConfig::getCity() { return selectedCity; }

std::vector<std::pair<DragInfo, QPoint>> YamlConfig::getItems() { return items; }
