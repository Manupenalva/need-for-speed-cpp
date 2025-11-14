#include "mapcanvas.h"

#include <QCoreApplication>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileInfo>
#include <QGraphicsItem>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QMimeData>
#include <QMouseEvent>
#include <QPen>
#include <QPixmap>
#include <QVBoxLayout>

#include "yaml_config.h"
#include "editor_constants.h"

MapCanvas::MapCanvas(QWidget* parent): QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setScene(scene);
    view->viewport()->installEventFilter(this);
    view->installEventFilter(this);
    controller = new SceneController(scene);

    view->setAcceptDrops(false);
    setAcceptDrops(true);

    layout->addWidget(view);

    saveButton = new QPushButton("Save Map", this);
    layout->addWidget(saveButton);
    layout->setAlignment(saveButton, Qt::AlignCenter);

    connect(saveButton, &QPushButton::clicked, this, [this]() {
        bool ok;
        QString fileName = QInputDialog::getText(
                this, "Save Map", "Enter map name:", QLineEdit::Normal, currentCityName, &ok);
        if (!ok || fileName.isEmpty()) {
            return;
        }
        if (controller->countItemsOfType("start") != MAX_PLAYERS) {
            QMessageBox::warning(this, "Starts missing", "It is neccessary to be 8 starts points.");
            return;
        }
        if (controller->countItemsOfType("finish") != MAX_FINISH) {
            QMessageBox::warning(this, "Finish missing", "It is neccessary to be 1 finish line.");
            return;
        }
        QString filePath = QString("../maps/%1.yaml").arg(fileName);
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
    if (selecting) {
        event->ignore();
        QMessageBox::warning(this, "No checkpoint selecting",
                             "You must click a checkpoint first o press ESC to cancel.");
        return;
    }
    if (event->mimeData()->hasFormat(DragInfo().mimeType())) {
        event->acceptProposedAction();
    }
}

void MapCanvas::dropEvent(QDropEvent* event) {
    if (selecting) {
        event->ignore();
        QMessageBox::warning(this, "No checkpoint selecting",
                             "You must click a checkpoint first o press ESC to cancel.");
        return;
    }
    DragInfo dragInfo;
    if (!dragInfo.unpack(event->mimeData()->data(dragInfo.mimeType()))) {
        return;
    }

    if (dragInfo.getType().contains("start", Qt::CaseInsensitive) &&
        controller->countItemsOfType("start") >= MAX_PLAYERS) {
        QMessageBox::warning(this, "Limit reach",
                             "There are 8 starting points in the map. Please remove one.");
        return;
    }

    if (dragInfo.getType().contains("finish", Qt::CaseInsensitive) &&
        controller->countItemsOfType("finish") >= MAX_FINISH) {
        QMessageBox::warning(
                this, "Limit reach",
                "There is a finish line in the map. Please remove to drop the new one.");
        return;
    }

    QPointF scenePos = view->mapToScene(event->position().toPoint());
    int x = static_cast<int>(scenePos.x() / GRID_SIZE) * GRID_SIZE;
    int y = static_cast<int>(scenePos.y() / GRID_SIZE) * GRID_SIZE;
    if (!dragInfo.getType().contains("hint", Qt::CaseInsensitive)) {
        controller->handleDropEvent(dragInfo, x, y);
        event->acceptProposedAction();
        return;
    }
    if (controller->countItemsOfType("checkpoint") == 0) {
        QMessageBox::warning(this, "No checkpoint",
                             "You must place at least one checkpoint before placing hints");
        return;
    }
    QMessageBox::information(this, "Select checkpoint",
                             "Select the checkpoint to associate this new hint. ESC to exit.");
    event->acceptProposedAction();
    selecting = true;
    info = dragInfo;
    hintPos = QPointF(x, y);
    view->setFocus();
}

void MapCanvas::exportToYaml(const QString& filePath) {
    YamlConfig yaml;
    yaml.save(scene, currentCityName, GRID_SIZE, filePath);
}

bool MapCanvas::eventFilter(QObject* obj, QEvent* event) {
    if (obj == view->viewport()) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (selecting && mouseEvent->button() == Qt::LeftButton) {
                QPointF scenePos = view->mapToScene(mouseEvent->pos());
                const auto items = scene->items(scenePos);
                for (auto* i: items) {
                    const auto t = i->data(TYPE).toString();
                    if (t.contains(CHECKPOINT_TYPE, Qt::CaseInsensitive)) {
                        controller->placeHint(info, hintPos, i);
                        selecting = false;
                        return true;
                    }
                }
                return true;
            }
            if (mouseEvent->button() == Qt::RightButton && !selecting) {
                QGraphicsItem* item =
                        scene->itemAt(view->mapToScene(mouseEvent->pos()), QTransform());
                if (item->data(TYPE).toString().contains(CHECKPOINT_TYPE, Qt::CaseInsensitive)) {
                    controller->deleteHints(item);
                    return true;
                }
                if (item && item->data(TYPE).isValid()) {
                    scene->removeItem(item);
                    delete item;
                    return true;
                }
            }
        }
        if (event->type() == QEvent::DragEnter || event->type() == QEvent::DragMove) {
            QDragEnterEvent* dragEvent = static_cast<QDragEnterEvent*>(event);
            dragEnterEvent(dragEvent);
            return true;
        }
        if (event->type() == QEvent::Drop) {
            QDropEvent* d = static_cast<QDropEvent*>(event);
            dropEvent(d);
            return true;
        }
    } else if (obj == view) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (selecting && keyEvent->key() == Qt::Key_Escape) {
                selecting = false;
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void MapCanvas::importFromYaml(const QString& filePath) {
    YamlConfig yaml;
    yaml.load(filePath);
    loadCityMap(QString(CITY_ASSETS_PATH + yaml.getCity() + ".png"));
    for (const auto& [i, pos]: yaml.getItems()) {
        controller->handleDropEvent(i, pos.x(), pos.y());
    }
}

