#ifndef MAPCANVAS_H
#define MAPCANVAS_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QWidget>
#include <QPointF>

#include "scene_controller.h"
#include "drag_info.h"

class MapCanvas: public QWidget {
    Q_OBJECT

public:
    explicit MapCanvas(QWidget* parent = nullptr);
    void loadCityMap(const QString& cityName);
    void exportToYaml(const QString& filePath);
    void importFromYaml(const QString& filePath);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    QGraphicsView* view;
    QGraphicsScene* scene;
    QGraphicsPixmapItem* mapItem;
    QString currentCityName;

    QPushButton* saveButton;

    SceneController* controller;
    bool selecting = false;
    DragInfo info;
    QPointF hintPos;
};

#endif
