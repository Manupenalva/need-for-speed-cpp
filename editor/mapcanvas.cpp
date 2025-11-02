#include "mapcanvas.h"

#include <QBrush>
#include <QCoreApplication>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsRectItem>
#include <QMimeData>
#include <QPen>
#include <QPixmap>
#include <QTextStream>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QCursor>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <yaml-cpp/yaml.h>
#include <vector>
#include <QMouseEvent>
#include <QKeyEvent>

#define GRID_SIZE 50

MapCanvas::MapCanvas(QWidget* parent): QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setScene(scene);
    view->viewport()->installEventFilter(this);

    view->setAcceptDrops(false);
    setAcceptDrops(true);

    layout->addWidget(view);

    saveButton = new QPushButton("Save Map", this);
    layout->addWidget(saveButton);
    layout->setAlignment(saveButton, Qt::AlignCenter);

    connect(saveButton, &QPushButton::clicked, this, [this]() {
        QString filePath = QString("./maps/%1.yaml").arg(currentCityName);
        exportToYaml(filePath);
        QCoreApplication::quit();
    });


    setLayout(layout);

    mapItem = nullptr;
}

void MapCanvas::loadCityMap(const QString& cityName) {
    scene->clear();

    QPixmap mapPixmap(cityName);
    mapItem = scene->addPixmap(mapPixmap);
    mapItem->setZValue(-1);

    QFileInfo fileInfo(cityName);
    currentCityName = fileInfo.baseName();

    QPen gridPen(Qt::lightGray);
    gridPen.setWidth(1);

    for (int x = 0; x < mapPixmap.width(); x += GRID_SIZE) {
        scene->addLine(x, 0, x, mapPixmap.height(), gridPen);
    }

    for (int y = 0; y < mapPixmap.height(); y += GRID_SIZE) {
        scene->addLine(0, y, mapPixmap.width(), y, gridPen);
    }

    scene->setSceneRect(0, 0, mapPixmap.width(), mapPixmap.height());
}

void MapCanvas::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasImage() || event->mimeData()->hasUrls() ||
        event->mimeData()->hasText()) {
        event->acceptProposedAction();
    }
}

void MapCanvas::dropEvent(QDropEvent* event) {
    QString nameOrPath;

    if (event->mimeData()->hasText())
        nameOrPath = event->mimeData()->text();

    QPixmap pixmap;
    if (event->mimeData()->hasImage())
        pixmap = qvariant_cast<QPixmap>(event->mimeData()->imageData());
    
    if (pixmap.isNull()) 
        return;

    QString type;
    QString rotation = "";
    if (nameOrPath.contains("checkpoint", Qt::CaseInsensitive))
        type = "checkpoint";
    else if (nameOrPath.contains("start", Qt::CaseInsensitive))
        type = "start";
    else if (nameOrPath.contains("finish", Qt::CaseInsensitive))
        type = "finish";
    else if (nameOrPath.contains("road", Qt::CaseInsensitive))
        type = "road";
    else if (nameOrPath.contains("hint", Qt::CaseInsensitive)) {
        type = "hint";
        if (nameOrPath.contains("Left", Qt::CaseInsensitive))
            rotation = "left";
        else if (nameOrPath.contains("Right", Qt::CaseInsensitive))
            rotation = "right";
        else if (nameOrPath.contains("Up", Qt::CaseInsensitive))
            rotation = "up";
        else if (nameOrPath.contains("Down", Qt::CaseInsensitive))
            rotation = "down";
    }
    else if (nameOrPath.contains("NPC", Qt::CaseInsensitive))
        type = "NPC";
    else
        type = "unknown";
    
    int startCount = 0;
    int finishCount = 0;

    for (QGraphicsItem* item : scene->items()) {
        QVariant data = item->data(0);
        if (!data.isValid())
            continue;
        QString itemType = data.toString();
        if (itemType == "start")
            startCount++;
        else if (itemType == "finish")
            finishCount++;
    }

    if (type == "start" && startCount >= 8) {
        QMessageBox::warning(this, "Limite alcanzado", "Ya hay 8 puntos de inicio en el mapa.");
        return;
    }

    if (type == "finish" && finishCount >= 1) {
        QMessageBox::warning(this, "Limite alcanzado", "Ya hay 1 punto de finalización en el mapa.");
        return;
    }

    QPointF scenePos = view->mapToScene(event->position().toPoint());
    qreal x = static_cast<int>(scenePos.x() / GRID_SIZE) * GRID_SIZE;
    qreal y = static_cast<int>(scenePos.y() / GRID_SIZE) * GRID_SIZE;

    auto* item = scene->addPixmap(
            pixmap.scaled(GRID_SIZE, GRID_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    item->setPos(x, y);
    item->setZValue(type == "road" ? 1 : 10);

    item->setData(0, type);
    item->setData(1, rotation); 

    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemSendsGeometryChanges);
    item->setCursor(Qt::OpenHandCursor);

    event->acceptProposedAction();
    
}

void MapCanvas::exportToYaml(const QString& filePath) {
    try {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "city" << YAML::Value << currentCityName.toStdString();
        out << YAML::Key << "width" << YAML::Value << static_cast<int>(scene->width());
        out << YAML::Key << "height" << YAML::Value << static_cast<int>(scene->height());
        out << YAML::Key << "celdWidth" << YAML::Value << GRID_SIZE;
        out << YAML::Key << "celdHeight" << YAML::Value << GRID_SIZE;
        
        auto writeElements = [&](const QString& elementType) {
            std::vector<YAML::Node> elements;
            for (QGraphicsItem* item: scene->items()) {
                if (!item->data(0).isValid())
                    continue;
                QString type = item->data(0).toString();
                if (type != elementType) continue;
                YAML::Node element;
                QPointF pos = item->pos();
                element["x"] = static_cast<int>(pos.x());
                element["y"] = static_cast<int>(pos.y());
                if (elementType == "hint") {
                    QString rotation = item->data(1).toString();
                    element["rotation"] = rotation.toStdString();
                }
                elements.push_back(element);
            }
            if (!elements.empty()) {
                out << YAML::Key << elementType.toStdString() << YAML::Value << YAML::BeginSeq;
                for (const auto& elem : elements) {
                    out << elem;
                }
                out << YAML::EndSeq;
            }
        };
        writeElements("start");
        writeElements("finish");
        writeElements("road");
        writeElements("checkpoint");
        writeElements("hint");
        writeElements("NPC");

        out << YAML::EndMap;

        QFile yamlFile(filePath);
        if (yamlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream yamlOut(&yamlFile);
            yamlOut << out.c_str();
            yamlFile.close();
        }
    } catch (const YAML::Exception& e) {
        qWarning("Error al generar el YAML: %s", e.what());
    }
}

bool MapCanvas::eventFilter(QObject* obj, QEvent* event) {
    if (obj == view->viewport()) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::RightButton) {
                QGraphicsItem* item = scene->itemAt(view->mapToScene(mouseEvent->pos()), QTransform());
                if (item && item->data(0).isValid()) {
                    scene->removeItem(item);
                    delete item;
                    return true; 
                }
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void MapCanvas::importFromYaml(const QString& filePath) {
    try {
        QFile yamlFile(filePath);
        if (!yamlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning("No se pudo abrir el archivo YAML: %s", filePath.toStdString().c_str());
            return;
        }

        QByteArray fileData = yamlFile.readAll();
        yamlFile.close();

        YAML::Node config = YAML::Load(fileData.constData());

        QString cityName = QString::fromStdString(config["city"].as<std::string>());
        loadCityMap(QString("./client/assets/cities/%1.png").arg(cityName));

        auto addElementsFromYaml = [&](const QString& elementType) {
            if (config[elementType.toStdString()]) {
                for (const auto& elem : config[elementType.toStdString()]) {
                    int x = elem["x"].as<int>();
                    int y = elem["y"].as<int>();
                    int rotationDeg = 0;
                    if (elementType == "hint" && elem["rotation"]) {
                        QString rotationStr = QString::fromStdString(elem["rotation"].as<std::string>());
                        if (rotationStr == "left")
                            rotationDeg = 270;
                        else if (rotationStr == "right")
                            rotationDeg = 180;
                        else if (rotationStr == "up")
                            rotationDeg = 90;
                        else
                            rotationDeg = 0;
                    }
                    addElement(elementType, x, y, rotationDeg);
                }
            }
        };

        addElementsFromYaml("start");
        addElementsFromYaml("finish");
        addElementsFromYaml("road");
        addElementsFromYaml("checkpoint");
        addElementsFromYaml("hint");
        addElementsFromYaml("NPC");

    } catch (const YAML::Exception& e) {
        qWarning("Error al leer el YAML: %s", e.what());
    }
}

void MapCanvas::addElement(const QString& type, int x, int y, int rotationDeg) {
    QString iconPath;
    if (type == "road") {
        iconPath = "./editor/imgs/road.png";
    } else if (type == "checkpoint") {
        iconPath = "./editor/imgs/checkpoint.png";
    } else if (type == "start") {
        iconPath = "./editor/imgs/start.png";
    } else if (type == "finish") {
        iconPath = "./editor/imgs/finish.png";
    } else if (type == "hint") {
        iconPath = "./editor/imgs/hint.png";
    } else if (type == "NPC") {
        iconPath = "./editor/imgs/npc.png";
    } else {
        return; 
    }

    QPixmap pixmap(iconPath);
    if (pixmap.isNull()) {
        qWarning("No se pudo cargar el ícono para el tipo: %s", type.toStdString().c_str());
        return;
    }

    if (rotationDeg != 0) {
        QTransform transform;
        transform.rotate(rotationDeg);
        pixmap = pixmap.transformed(transform, Qt::SmoothTransformation);
    }
    auto* item = scene->addPixmap(
            pixmap.scaled(GRID_SIZE, GRID_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    item->setPos(x, y);
    item->setZValue(type == "road" ? 1 : 10);
    item->setData(0, type);
    if (type == "hint") {
        QString rotationStr;
        if (rotationDeg == 270)
            rotationStr = "left";
        else if (rotationDeg == 180)
            rotationStr = "right";
        else if (rotationDeg == 90)
            rotationStr = "up";
        else
            rotationStr = "down";
        item->setData(1, rotationStr);
    }
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemSendsGeometryChanges);
    item->setCursor(Qt::OpenHandCursor);
}   