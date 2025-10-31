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

#define GRID_SIZE 50

MapCanvas::MapCanvas(QWidget* parent): QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setScene(scene);

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
    if (type == "road")
        item->setZValue(1);
    else
        item->setZValue(10);

    item->setData(0, type);
    item->setData(1, rotation); 

    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemSendsGeometryChanges);
    item->setCursor(Qt::OpenHandCursor);

    event->acceptProposedAction();
    
}

void MapCanvas::exportToYaml(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("No se pudo abrir el archivo para escribir");
        return;
    }

    QTextStream out(&file);
    out << "city: " << currentCityName << "\n";
    out << "width: " << scene->width() << "\n";
    out << "height: " << scene->height() << "\n";
    out << "celdWidth: " << GRID_SIZE << "\n";
    out << "celdHeight: " << GRID_SIZE << "\n";

    auto writeElements = [&](const QString& elementType) {
        QList<QGraphicsItem*> itemsList = scene->items();
        bool wroteHeader = false;

        for (QGraphicsItem* item: itemsList) {
            QVariant data = item->data(0);
            if (!data.isValid())
                continue;

            QString type = data.toString();
            if (type == elementType) {
                if (!wroteHeader) {
                    out << "-" << elementType << ":\n";
                    wroteHeader = true;
                }
                QPointF pos = item->pos();
                out << "  x: " << static_cast<int>(pos.x()) << "\n";
                out << "  y: " << static_cast<int>(pos.y()) << "\n";
                if (elementType == "hint") {
                    QString rotation = item->data(1).toString();
                    out << "  rotation: " << rotation << "\n";
                }
            }
        }
    };

    writeElements("start");
    writeElements("finish");
    writeElements("road");
    writeElements("checkpoint");
    writeElements("hint");
    writeElements("NPC");

    file.close();
}
