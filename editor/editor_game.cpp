#include "editor_game.h"

#include <QDrag>
#include <QFileDialog>
#include <QGraphicsDropShadowEffect>
#include <QKeySequence>
#include <QMessageBox>
#include <QMimeData>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QShortcut>
#include <QTransform>

#include "cityselection.h"
#include "drag_info.h"
#include "mapcanvas.h"
#include "ui_EditorGame.h"

EditorGame::EditorGame(QWidget* parent): QMainWindow(parent), ui(new Ui::EditorGame) {
    ui->setupUi(this);
    toolsConstants();

    setUpNav();
    setUpTools();

    auto* esc = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(esc, &QShortcut::activated, this, [this]() {
        if (ui->mapCanvas) {
            ui->mapCanvas->cancelSelecting();
        }
    });

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
    const auto path =
            QFileDialog::getOpenFileName(this, "Open YAML", SAVE_MAP, "YAML (*.yaml *.yml)");
    if (path.isEmpty())
        return;
    ui->mapCanvas->importFromYaml(path);
    ui->stackedWidget->setCurrentWidget(ui->pageEditor);
}

void EditorGame::setUpTools() {
    for (const auto& t: tools) {
        rotateIcon(t.btn, t.iconPath, t.rotation);
        dragMovement(t.btn, t.type, t.iconPath, t.rotation);
    }
}

void EditorGame::dragMovement(QToolButton* btn, const QString& type, const QString& iconPath,
                              int rotDeg) {
    if (!btn)
        return;
    QObject::connect(btn, &QToolButton::pressed, this, [this, btn, type, iconPath, rotDeg] {
        if (ui->mapCanvas && ui->mapCanvas->isSelecting()) {
            QMessageBox::information(this, "Select checkpoint",
                                     "You must select a checkpoint first. ESC to exit.");
            return;
        }
        DragInfo d(type, rotDeg, iconPath);
        auto* mime = new QMimeData;
        mime->setData(d.mimeType(), d.pack());
        QDrag drag(btn);
        drag.setMimeData(mime);

        QPixmap px(iconPath);
        if (!px.isNull() && rotDeg) {
            QTransform t;
            t.rotate(rotDeg);
            px = px.transformed(t, Qt::SmoothTransformation);
        }
        drag.setPixmap(px.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        drag.exec(Qt::CopyAction);
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

void EditorGame::toolsConstants() {
    tools = {
            {ui->toolStartLine, LINE_TYPE, LINE_PATH, 0},
            {ui->toolStartLineVertical, LINE_TYPE, LINE_PATH, LINE_ROTATION},

            {ui->toolCheckpointVertical, CHECKPOINT_TYPE, CHECKPOINT_PATH, 0},
            {ui->toolCheckpointHorizontal, CHECKPOINT_TYPE, CHECKPOINT_PATH, HORIZONTAL_ROTATION},

            {ui->toolStartUp, START_TYPE, START_PATH_1, 0},
            {ui->toolStartLeft, START_TYPE, START_PATH_2, START_LEFT},
            {ui->toolStartDown, START_TYPE, START_PATH_1, START_DOWN},
            {ui->toolStartRight, START_TYPE, START_PATH_2, START_RIGHT},

            {ui->toolFinish, FINISH_TYPE, FINISH_PATH, 0},
            {ui->toolFinishHorizontal, FINISH_TYPE, FINISH_PATH, HORIZONTAL_ROTATION},

            {ui->toolHintLeft, HINT_TYPE, HINT_PATH, 0},
            {ui->toolHintDown, HINT_TYPE, HINT_PATH, DOWN_ROTATION},
            {ui->toolHintUp, HINT_TYPE, HINT_PATH, UP_ROTATION},
            {ui->toolHintRight, HINT_TYPE, HINT_PATH, RIGHT_ROTATION},
    };
}
