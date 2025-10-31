#ifndef MAPCANVAS_H
#define MAPCANVAS_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QPushButton>
#include <QWidget>

class MapCanvas: public QWidget {
    Q_OBJECT

public:
    explicit MapCanvas(QWidget* parent = nullptr);
    void loadCityMap(const QString& cityName);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private:
    QGraphicsView* view;
    QGraphicsScene* scene;
    QGraphicsPixmapItem* mapItem;
    QString currentCityName;

    QPushButton* saveButton;
    void exportToYaml(const QString& filePath);
};

#endif
