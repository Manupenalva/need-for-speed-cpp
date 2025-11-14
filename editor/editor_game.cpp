#include "editor_game.h"

#include <QDrag>
#include <QFileDialog>
#include <QMimeData>
#include <QPixmap>
#include <QToolButton>
#include <QTransform>

#include "cityselection.h"
#include "drag_info.h"
#include "mapcanvas.h"
#include "ui_EditorGame.h"

EditorGame::EditorGame(QWidget* parent): QMainWindow(parent), ui(new Ui::EditorGame) {
    ui->setupUi(this);

    setUpNav();
    setUpTools();

    if (ui->citySelection) {
        connect(ui->citySelection, &CitySelection::citySelected, this,
                [this](const QString& filePath) {
                    ui->mapCanvas->loadCityMap(filePath);
                    ui->stackedWidget->setCurrentWidget(ui->pageEditor);
                });
    }
}

EditorGame::~EditorGame() { delete ui; }

void EditorGame::setUpNav() {
    connect(ui->btnCreateCity, &QPushButton::clicked,
            [this] { ui->stackedWidget->setCurrentWidget(ui->pageCitySelection); });
    connect(ui->btnLoadMap, &QPushButton::clicked, this, &EditorGame::setUpLoad);
    connect(ui->btnBackFromCity, &QPushButton::clicked,
            [this] { ui->stackedWidget->setCurrentWidget(ui->pageMainMenu); });
}

void EditorGame::setUpLoad() {
    const auto path = QFileDialog::getOpenFileName(this, "Open YAML", ".", "YAML (*.yaml *.yml)");
    if (path.isEmpty())
        return;
    ui->mapCanvas->importFromYaml(path);
    ui->stackedWidget->setCurrentWidget(ui->pageEditor);
}

void EditorGame::setUpTools() {
    rotateIcon(ui->toolHintLeft, "../editor/imgs/hint.png", 0);
    rotateIcon(ui->toolHintDown, "../editor/imgs/hint.png", 270);
    rotateIcon(ui->toolHintUp, "../editor/imgs/hint.png", 90);
    rotateIcon(ui->toolHintRight, "../editor/imgs/hint.png", 180);

    dragMovement(ui->toolRoad, "road", 0, "../editor/imgs/road.png");
    dragMovement(ui->toolRoad1, "road1", 0, "../editor/imgs/road1.png");
    dragMovement(ui->toolRoad2, "road2", 0, "../editor/imgs/road2.png");
    dragMovement(ui->toolCheckpointVertical, "checkpoint", 0, "../editor/imgs/checkpoint.png");
    dragMovement(ui->toolCheckpointHorizontal, "checkpoint", 0, "../editor/imgs/checkpoint.png");
    dragMovement(ui->toolStart, "start", 0, "../editor/imgs/start.png");
    dragMovement(ui->toolStart2, "start2", 0, "../editor/imgs/start2.png");
    dragMovement(ui->toolFinish, "finish", 0, "../editor/imgs/finish.png");
    dragMovement(ui->toolHintLeft, "hint", 0, "../editor/imgs/hint.png");
    dragMovement(ui->toolHintDown, "hint", 270, "../editor/imgs/hint.png");
    dragMovement(ui->toolHintUp, "hint", 90, "../editor/imgs/hint.png");
    dragMovement(ui->toolHintRight, "hint", 180, "../editor/imgs/hint.png");
}

void EditorGame::dragMovement(QToolButton* btn, const QString& type, int rotDeg,
                              const QString& iconPath) {
    if (!btn)
        return;
    QObject::connect(btn, &QToolButton::pressed, btn, [=] {
        DragInfo d(type, rotDeg, iconPath);
        auto* mime = new QMimeData;
        mime->setData(d.mimeType(), d.pack());
        auto* drag = new QDrag(btn);
        drag->setMimeData(mime);

        QPixmap px(iconPath);
        if (!px.isNull() && rotDeg) {
            QTransform t;
            t.rotate(rotDeg);
            px = px.transformed(t, Qt::SmoothTransformation);
        }
        drag->setPixmap(px.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        drag->exec(Qt::CopyAction);
    });
}

void EditorGame::rotateIcon(QToolButton* btn, const QString& iconPath, int rotDeg) {
    QPixmap base(iconPath);
    if (!base.isNull() && rotDeg) {
        QTransform t;
        t.rotate(rotDeg);
        base = base.transformed(t, Qt::SmoothTransformation);
    }
    btn->setIcon(base);
    btn->setIconSize(QSize(48, 48));
}
