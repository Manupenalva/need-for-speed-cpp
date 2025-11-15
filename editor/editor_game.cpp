#include "editor_game.h"

#include <QDrag>
#include <QFileDialog>
#include <QMimeData>
#include <QPixmap>
#include <QToolButton>
#include <QTransform>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>

#include "cityselection.h"
#include "drag_info.h"
#include "mapcanvas.h"
#include "ui_EditorGame.h"
#include "editor_constants.h"

EditorGame::EditorGame(QWidget* parent): QMainWindow(parent), ui(new Ui::EditorGame) {
    ui->setupUi(this);

    auto* glow = new QGraphicsDropShadowEffect(this);
    glow->setBlurRadius(40);
    glow->setColor(QColor(255, 40, 40));
    glow->setOffset(0, 0);               
    ui->titleLabelMenu->setGraphicsEffect(glow);

    auto* anim = new QPropertyAnimation(glow, "blurRadius", this);
    anim->setDuration(1500);
    anim->setStartValue(20);
    anim->setEndValue(60);
    anim->setEasingCurve(QEasingCurve::InOutSine);
    anim->setLoopCount(-1);
    anim->start();

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
    rotateIcon(ui->toolHintDown, HINT_PATH, DOWN_ROTATION);
    rotateIcon(ui->toolHintUp, HINT_PATH, UP_ROTATION);
    rotateIcon(ui->toolHintRight, HINT_PATH, RIGHT_ROTATION);
    rotateIcon(ui->toolCheckpointHorizontal, CHECKPOINT_PATH, HORIZONTAL_ROTATION);
    rotateIcon(ui->toolStartLeft, START_PATH_2, START_LEFT);
    rotateIcon(ui->toolStartRight, START_PATH_2, START_RIGHT);
    rotateIcon(ui->toolStartDown, START_PATH_1, START_DOWN);

    dragMovement(ui->toolRoad, ROAD_TYPE, ROAD_PATH_1);
    dragMovement(ui->toolRoad1, ROAD_TYPE, ROAD_PATH_2);
    dragMovement(ui->toolRoad2, ROAD_TYPE, ROAD_PATH_3);
    dragMovement(ui->toolCheckpointVertical, CHECKPOINT_TYPE, CHECKPOINT_PATH);
    dragMovement(ui->toolCheckpointHorizontal, CHECKPOINT_TYPE, CHECKPOINT_PATH, HORIZONTAL_ROTATION);
    dragMovement(ui->toolStartUp, START_TYPE, START_PATH_1);
    dragMovement(ui->toolStartLeft, START_TYPE, START_PATH_2, START_LEFT);
    dragMovement(ui->toolStartDown, START_TYPE, START_PATH_1, START_DOWN);
    dragMovement(ui->toolStartRight, START_TYPE, START_PATH_2, START_RIGHT);
    dragMovement(ui->toolFinish, FINISH_TYPE, FINISH_PATH);
    dragMovement(ui->toolHintLeft, HINT_TYPE, HINT_PATH);
    dragMovement(ui->toolHintDown, HINT_TYPE, HINT_PATH, DOWN_ROTATION);
    dragMovement(ui->toolHintUp, HINT_TYPE, HINT_PATH, UP_ROTATION);
    dragMovement(ui->toolHintRight, HINT_TYPE, HINT_PATH, RIGHT_ROTATION);
}

void EditorGame::dragMovement(QToolButton* btn, const QString& type,
                              const QString& iconPath, int rotDeg) {
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
