#include "editor_game.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLabel>
#include <QMimeData>
#include <QVBoxLayout>
#include <vector>
#include <QScrollArea>
#include <QTransform>

EditorGame::EditorGame(QWidget* parent): QMainWindow(parent) {
    stackedWidget = new QStackedWidget(this);

    mainMenuWidget = new QWidget(this);
    QVBoxLayout* mainMenuLayout = new QVBoxLayout(mainMenuWidget);

    QLabel* titleLabel = new QLabel("Editor Main Menu", mainMenuWidget);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    QPushButton* selectCityButton = new QPushButton("Create City Map");
    selectCityButton->setFixedHeight(50);

    mainMenuLayout->addWidget(titleLabel);
    mainMenuLayout->addSpacing(20);
    mainMenuLayout->addWidget(selectCityButton);
    mainMenuLayout->setAlignment(selectCityButton, Qt::AlignCenter);
    mainMenuWidget->setLayout(mainMenuLayout);


    connect(selectCityButton, &QPushButton::clicked, this, &EditorGame::goToCitySelection);

    citySelectionWidget = new CitySelection(this);
    connect(citySelectionWidget, &CitySelection::citySelected, this,
            &EditorGame::operEditorWithCity);

    editorWidget = new MapCanvas(this);

    stackedWidget->addWidget(mainMenuWidget);
    stackedWidget->addWidget(citySelectionWidget);
    stackedWidget->addWidget(editorWidget);

    setCentralWidget(stackedWidget);
    stackedWidget->setCurrentWidget(mainMenuWidget);

    resize(1280, 720);
}

EditorGame::~EditorGame() {}

void EditorGame::goToCitySelection() { stackedWidget->setCurrentWidget(citySelectionWidget); }

void EditorGame::operEditorWithCity(const QString& cityName) {
    QWidget* editorContainer = new QWidget(this);
    QHBoxLayout* editorLayout = new QHBoxLayout(editorContainer);

    QWidget* toolsWidget = new QWidget();
    QVBoxLayout* toolsLayout = new QVBoxLayout(toolsWidget);
    toolsLayout->setAlignment(Qt::AlignTop);

    QLabel* toolsTitle = new QLabel("Tools", toolsWidget);
    toolsTitle->setAlignment(Qt::AlignCenter);
    toolsTitle->setStyleSheet("font-size: 18px; font-weight: bold;");
    toolsLayout->addWidget(toolsTitle);
    toolsLayout->addSpacing(10);

    struct Tool {
        QString name;
        QString iconPath;
        qreal rotation = 0;
    };

    std::vector<Tool> tools = {
            {"Add Road", "./editor/imgs/road.png"},
            {"Add Road", "./editor/imgs/road1.png"},
            {"Add Road", "./editor/imgs/road2.png"},
            {"Add Checkpoint", "./editor/imgs/checkpoint.png"},
            {"Add Start Line", "./editor/imgs/start.png"},
            {"Add Start Line", "./editor/imgs/start2.png"},
            {"Add Finish Line", "./editor/imgs/finish.png"},
            {"Add Hint Left", "./editor/imgs/hint.png"},
            {"Add Hint Down", "./editor/imgs/hint.png", 270},
            {"Add Hint Up", "./editor/imgs/hint.png", 90},
            {"Add Hint Right", "./editor/imgs/hint.png", 180},
            {"Add NPC", "./editor/imgs/npc.png"},
    };

    for (auto& t: tools) {
        QPushButton* toolButton = new QPushButton();
        QPixmap base(t.iconPath);
        if (!base.isNull() && t.rotation != 0) {
            QTransform transform;
            transform.rotate(t.rotation);
            base = base.transformed(transform, Qt::SmoothTransformation);
        }
        toolButton->setIcon(base);
        toolButton->setIconSize(QSize(48, 48));
        toolButton->setFixedSize(64, 64);
        toolButton->setToolTip(t.name);
        QObject::connect(toolButton, &QPushButton::pressed, [toolButton, t, base]() {
            QMimeData* mimeData = new QMimeData;
            mimeData->setText(t.name);
            mimeData->setImageData(base);
            mimeData->setData("application/x-rotation", QByteArray::number(static_cast<int>(t.rotation)));
            QDrag* drag = new QDrag(toolButton);
            drag->setMimeData(mimeData);
            drag->setPixmap(base.scaled(48, 48, Qt::KeepAspectRatio));
            drag->exec(Qt::CopyAction);
        });

        toolsLayout->addWidget(toolButton);
    }

    QScrollArea* scrollArea = new QScrollArea(editorContainer);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(toolsWidget);
    scrollArea->setFixedWidth(140);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    editorWidget = new MapCanvas(editorContainer);
    editorWidget->loadCityMap(cityName);
    editorLayout->addWidget(scrollArea);
    editorLayout->addWidget(editorWidget, 1);
    editorLayout->setStretch(0, 0);
    editorLayout->setStretch(1, 1);
    editorContainer->setLayout(editorLayout);

    stackedWidget->addWidget(editorContainer);
    stackedWidget->setCurrentWidget(editorContainer);
}
