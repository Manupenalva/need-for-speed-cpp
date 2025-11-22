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
#include <QRectF>
#include <QVBoxLayout>

#include "yaml_config.h"

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
    setActions();

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
        if (controller->countItemsOfType(START_TYPE) != MAX_PLAYERS) {
            QMessageBox::warning(this, "Cars missing", "It is neccessary to be 8 cars points.");
            return;
        }
        if (controller->countItemsOfType(FINISH_TYPE) != MAX_FINISH) {
            QMessageBox::warning(this, "Finish missing", "It is neccessary to be 1 finish line.");
            return;
        }
        QString filePath = QString("%1/%2.yaml").arg(SAVE_MAP, fileName);
        exportToYaml(filePath);
        QMessageBox::information(this, "Map Saved",
                                 QString("Map saved successfully in:\n %1!").arg(filePath));
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
    DragInfo dragInfo;
    if (!dragInfo.unpack(event->mimeData()->data(dragInfo.mimeType()))) {
        event->ignore();
        return;
    }

    if (selecting) {
        event->ignore();
        QMessageBox::warning(this, "No checkpoint selecting",
                             "You must click a checkpoint first o press ESC to cancel.");
        return;
    }

    QPointF scenePos = view->mapToScene(event->position().toPoint());
    int x = static_cast<int>(scenePos.x() / GRID_SIZE) * GRID_SIZE;
    int y = static_cast<int>(scenePos.y() / GRID_SIZE) * GRID_SIZE;

    auto it = actions.find(dragInfo.getType());
    ActionResult actionResult = it->second->execute(*controller, dragInfo, x, y);
    if (!actionResult.success) {
        QMessageBox::warning(this, actionResult.errorTitle, actionResult.errorMessage);
        event->ignore();
        return;
    }

    if (actionResult.selecting) {
        selecting = true;
        info = actionResult.pending;
        hintPos = actionResult.pos;
        QMessageBox::information(this, "Select checkpoint",
                                 "Select the checkpoint to associate this new hint. ESC to exit.");
        view->setFocus();
    }
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
            QPoint pos = mouseEvent->pos();
            if (selecting && mouseEvent->button() == Qt::LeftButton) {
                handleSelectingHint(pos);
                return true;
            }
            if (mouseEvent->button() == Qt::RightButton && !selecting) {
                handleDelete(pos);
                return true;
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
            if (keyEvent->key() == Qt::Key_Plus) {
                zoomIn();
                return true;
            }
            if (keyEvent->key() == Qt::Key_Minus) {
                zoomOut();
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
    for (const auto& i: yaml.getItems()) {
        controller->handleDropEvent(i.info, i.pos.x(), i.pos.y());
    }
}

void MapCanvas::setActions() {
    actions.emplace(QStringLiteral(LINE_TYPE), std::make_unique<ActionStartLine>());
    actions.emplace(QStringLiteral(CHECKPOINT_TYPE), std::make_unique<ActionCheckpoint>());
    actions.emplace(QStringLiteral(START_TYPE), std::make_unique<ActionStart>());
    actions.emplace(QStringLiteral(FINISH_TYPE), std::make_unique<ActionFinish>());
    actions.emplace(QStringLiteral(HINT_TYPE), std::make_unique<ActionHint>());
}

void MapCanvas::handleSelectingHint(const QPoint& pos) {
    QPointF scenePos = view->mapToScene(pos);
    QRectF pickArea(scenePos.x() - GRID_SIZE / 2.0, scenePos.y() - GRID_SIZE / 2.0, GRID_SIZE,
                    GRID_SIZE);
    auto items = scene->items(pickArea);
    for (auto* i: items) {
        auto t = i->data(TYPE).toString();
        if (t.contains(CHECKPOINT_TYPE, Qt::CaseInsensitive)) {
            controller->placeHint(info, hintPos, i);
            selecting = false;
            break;
        }
    }
}

void MapCanvas::handleDelete(const QPoint& pos) {
    QPointF scenePos = view->mapToScene(pos);
    QRectF pickArea(scenePos.x() - GRID_SIZE / 2.0, scenePos.y() - GRID_SIZE / 2.0, GRID_SIZE,
                    GRID_SIZE);
    auto items = scene->items(pickArea);
    if (items.isEmpty()) {
        return;
    }
    QGraphicsItem* item = items.first();
    if (!item->data(TYPE).isValid())
        return;
    controller->deleteItem(item);
}

void MapCanvas::zoomIn() {
    double newZoom = currentZoom + ZOOM_SCALE;
    if (newZoom > MAX_ZOOM)
        newZoom = MAX_ZOOM;

    double factor = newZoom / currentZoom;
    if (factor != 1.0) {
        view->scale(factor, factor);
        currentZoom = newZoom;
    }
}

void MapCanvas::zoomOut() {
    double newZoom = currentZoom - ZOOM_SCALE;
    if (newZoom < MIN_ZOOM)
        newZoom = MIN_ZOOM;

    double factor = newZoom / currentZoom;
    if (factor != 1.0) {
        view->scale(factor, factor);
        currentZoom = newZoom;
    }
}
