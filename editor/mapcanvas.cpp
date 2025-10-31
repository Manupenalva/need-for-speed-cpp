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
        QString filePath = QString("/home/facu/Escritorio/%1.yaml").arg(currentCityName);
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

    int gridSize = 50;
    QPen gridPen(Qt::lightGray);
    gridPen.setWidth(1);

    for (int x = 0; x < mapPixmap.width(); x += gridSize) {
        scene->addLine(x, 0, x, mapPixmap.height(), gridPen);
    }

    for (int y = 0; y < mapPixmap.height(); y += gridSize) {
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

    if (pixmap.isNull() && !nameOrPath.isEmpty()) {
        if (nameOrPath.contains("checkpoint", Qt::CaseInsensitive))
            pixmap.load("/home/facu/Imágenes/checkpoint.png");
        else if (nameOrPath.contains("start", Qt::CaseInsensitive))
            pixmap.load("/home/facu/Imágenes/start.png");
        else if (nameOrPath.contains("finish", Qt::CaseInsensitive))
            pixmap.load("/home/facu/Imágenes/finish.png");
        else if (nameOrPath.contains("road", Qt::CaseInsensitive))
            pixmap.load("/home/facu/Imágenes/road.png");
    }

    if (!pixmap.isNull()) {
        QPointF scenePos = view->mapToScene(event->position().toPoint());
        int gridSize = 50;
        qreal x = static_cast<int>(scenePos.x() / gridSize) * gridSize;
        qreal y = static_cast<int>(scenePos.y() / gridSize) * gridSize;

        auto* item = scene->addPixmap(
                pixmap.scaled(gridSize, gridSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        item->setPos(x, y);
        item->setZValue(10);

        QString type;
        if (nameOrPath.contains("checkpoint", Qt::CaseInsensitive))
            type = "checkpoint";
        else if (nameOrPath.contains("start", Qt::CaseInsensitive))
            type = "start";
        else if (nameOrPath.contains("finish", Qt::CaseInsensitive))
            type = "finish";
        else if (nameOrPath.contains("road", Qt::CaseInsensitive))
            type = "road";
        else
            type = "unknown";

        item->setData(0, type);

        event->acceptProposedAction();
    }
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
    out << "celdWidth: " << 50 << "\n";
    out << "celdHeight: " << 50 << "\n";

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
            }
        }
    };

    writeElements("start");
    writeElements("finish");
    writeElements("road");
    writeElements("checkpoint");
    writeElements("hint");

    file.close();
}
