#include "yaml_config.h"

#include <QFile>
#include <QGraphicsItem>
#include <string>
#include <vector>
#include "editor_constants.h"

YamlConfig::YamlConfig() {
    setActions();
}

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

        writeElements(out, scene, START_TYPE);
        writeElements(out, scene, FINISH_TYPE);
        writeElements(out, scene, ROAD_TYPE);
        writeElements(out, scene, CHECKPOINT_TYPE);
        writeElements(out, scene, HINT_TYPE);

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
    auto it = actions.find(elementType);
    if (it == actions.end()) {
        return;
    }
    Actions* action = it->second.get();
    std::vector<YAML::Node> elements;

    for (QGraphicsItem* item: scene->items(Qt::AscendingOrder)) {
        if (!item->data(TYPE).isValid() ||
            !item->data(TYPE).toString().contains(elementType, Qt::CaseInsensitive)) {
            continue;
        }
        YAML::Node element = action->writeNode(item);
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
        addElements(config, START_TYPE);
        addElements(config, FINISH_TYPE);
        addElements(config, ROAD_TYPE);
        addElements(config, CHECKPOINT_TYPE);
        addElements(config, HINT_TYPE);
        return true;
    } catch (const YAML::Exception& e) {
        qWarning("Error al leer el YAML: %s", e.what());
        return false;
    }
}

void YamlConfig::addElements(const YAML::Node& config, const QString& elementType) {
    auto it = actions.find(elementType);
    if (it == actions.end()) {
        return;
    }
    Actions* action = it->second.get();
    auto node = config[elementType.toStdString()];
    if (!node) {
        return;
    }

    for (const auto& elem: node) {
        ItemRecord info = action->loadNode(elem);
        items.emplace_back(info);
    }
    
}

QString YamlConfig::getCity() { return selectedCity; }

std::vector<ItemRecord> YamlConfig::getItems() { return items; }

void YamlConfig::setActions() {
    actions.emplace(QStringLiteral(ROAD_TYPE), std::make_unique<ActionRoad>());
    actions.emplace(QStringLiteral(CHECKPOINT_TYPE), std::make_unique<ActionCheckpoint>());
    actions.emplace(QStringLiteral(START_TYPE), std::make_unique<ActionStart>());
    actions.emplace(QStringLiteral(FINISH_TYPE), std::make_unique<ActionFinish>());
    actions.emplace(QStringLiteral(HINT_TYPE), std::make_unique<ActionHint>());
}
