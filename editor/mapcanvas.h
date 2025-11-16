#ifndef MAPCANVAS_H
#define MAPCANVAS_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPointF>
#include <QPushButton>
#include <QWidget>
#include <map>
#include <memory>

#include "drag_info.h"
#include "scene_controller.h"
#include "editor_constants.h"
#include "./actions/actions.h"
#include "./actions/action_checkpoint.h"
#include "./actions/action_start.h"
#include "./actions/action_finish.h"
#include "./actions/action_road.h"
#include "./actions/action_hint.h"

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

    std::map<QString, std::unique_ptr<Actions>> actions;
    void setActions();

    SceneController* controller;
    bool selecting = false;
    DragInfo info;
    QPointF hintPos;
    double currentZoom = INITIAL_ZOOM;
};

#endif
