#ifndef YAML_CONFIG_H
#define YAML_CONFIG_H

#include <QGraphicsScene>
#include <QString>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "./actions/action_checkpoint.h"
#include "./actions/action_finish.h"
#include "./actions/action_hint.h"
#include "./actions/action_start.h"
#include "./actions/action_start_line.h"
#include "./actions/actions.h"

#include "drag_info.h"

class YamlConfig {
private:
    void writeElements(YAML::Emitter& out, const QGraphicsScene* scene, const QString& elementType);
    void addElements(const YAML::Node& out, const QString& elementType);
    QString selectedCity;
    std::vector<ItemRecord> items;
    std::map<QString, std::unique_ptr<Actions>> actions;

    void setActions();

public:
    YamlConfig();

    bool save(const QGraphicsScene* scene, const QString& city, int gridSize, const QString& path);
    bool load(const QString& path);
    QString getCity();
    std::vector<ItemRecord> getItems();
};
#endif
