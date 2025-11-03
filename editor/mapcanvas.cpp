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
#include <QLineEdit>
#include <QInputDialog>
#include "scene_controller.h"
#include "drag_info.h"
#include "yaml_config.h"

#define GRID_SIZE 50

MapCanvas::MapCanvas(QWidget* parent): QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setScene(scene);
    view->viewport()->installEventFilter(this);
    controller = new SceneController(scene);

    view->setAcceptDrops(false);
    setAcceptDrops(true);

    layout->addWidget(view);

    saveButton = new QPushButton("Save Map", this);
    layout->addWidget(saveButton);
    layout->setAlignment(saveButton, Qt::AlignCenter);

    connect(saveButton, &QPushButton::clicked, this, [this]() {
        bool ok;
        QString fileName = QInputDialog::getText(this, "Save Map", "Enter map name:",
                                                 QLineEdit::Normal, currentCityName, &ok);
        if (controller->countItemsOfType("start") != 8) {
            QMessageBox::warning(this, "Faltan starts", "Es necesario 8 starts.");
            return;
        }
        if (controller->countItemsOfType("finish") != 1){
            QMessageBox::warning(this, "Faltan finishes", "Es necesario 1 finish.");
            return;
        }
        if (!ok || fileName.isEmpty()) {
            return;
        }
        QString filePath = QString("./maps/%1.yaml").arg(fileName);
        exportToYaml(filePath);
        QMessageBox::information(this, "Map Saved", "Map saved successfully!");
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
    if (event->mimeData()->hasFormat(DragInfo().mimeType())) {
        event->acceptProposedAction();
    }
}

void MapCanvas::dropEvent(QDropEvent* event) {
    DragInfo dragInfo;
    if (!dragInfo.unpack(event->mimeData()->data(dragInfo.mimeType()))) {
        return;
    }

    if (dragInfo.getType().contains("start", Qt::CaseInsensitive) && controller->countItemsOfType("start") >= 8) {
        QMessageBox::warning(this, "Limite alcanzado", "Ya hay 8 puntos de inicio en el mapa.");
        return;
    }

    if (dragInfo.getType().contains("finish", Qt::CaseInsensitive) && controller->countItemsOfType("finish") >= 1) {
        QMessageBox::warning(this, "Limite alcanzado", "Ya hay 1 punto de finalización en el mapa.");
        return;
    }

    QPointF scenePos = view->mapToScene(event->position().toPoint());
    int x = static_cast<int>(scenePos.x() / GRID_SIZE) * GRID_SIZE;
    int y = static_cast<int>(scenePos.y() / GRID_SIZE) * GRID_SIZE;
    controller->handleDropEvent(dragInfo, x, y, false);
    event->acceptProposedAction();
}

void MapCanvas::exportToYaml(const QString& filePath) {
    YamlConfig yaml;
    yaml.save(scene, currentCityName, GRID_SIZE, filePath);
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
    YamlConfig yaml;
    yaml.load(filePath);
    loadCityMap(QString("./client/assets/cities/%1.png").arg(yaml.getCity()));
    for (const auto& [i, pos] : yaml.getItems()) {
        controller->handleDropEvent(i, pos.x(), pos.y(), false);
    }
}