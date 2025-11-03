#ifndef YAML_CONFIG_H
#define YAML_CONFIG_H

#include <QGraphicsScene>
#include <QString>
#include <yaml-cpp/yaml.h>
#include <vector>
#include "drag_info.h"
#include <utility>

class YamlConfig {
private:
    void writeElements(YAML::Emitter& out, const QGraphicsScene* scene, const QString& elementType);
    void addElements(YAML::Node& out, const QString& elementType);
    QString selectedCity;
    std::vector<std::pair<DragInfo, QPoint>> items;
public:
    YamlConfig() = default;

    bool save(const QGraphicsScene* scene, const QString& city, int gridSize, const QString& path);
    bool load(const QString& path);
    QString getCity();
    std::vector<std::pair<DragInfo, QPoint>> getItems();
};
#endif