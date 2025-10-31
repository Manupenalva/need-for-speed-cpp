#include "editor_game.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLabel>
#include <QMimeData>
#include <QVBoxLayout>
#include <vector>

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

    resize(800, 600);
}

EditorGame::~EditorGame() {}

void EditorGame::goToCitySelection() { stackedWidget->setCurrentWidget(citySelectionWidget); }

void EditorGame::operEditorWithCity(const QString& cityName) {
    QWidget* editorContainer = new QWidget(this);
    QHBoxLayout* editorLayout = new QHBoxLayout(editorContainer);

    QWidget* toolsWidget = new QWidget(editorContainer);
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
    };

    std::vector<Tool> tools = {
            {"Add Road", "/home/facu/Imágenes/road.png"},
            {"Add Checkpoint", "/home/facu/Imágenes/checkpoint.png"},
            {"Add Start Line", "/home/facu/Imágenes/start.png"},
            {"Add Finish Line", "/home/facu/Imágenes/finish.png"},
    };

    for (auto& t: tools) {
        QPushButton* toolButton = new QPushButton();
        toolButton->setIcon(QIcon(t.iconPath));
        toolButton->setIconSize(QSize(48, 48));
        toolButton->setFixedSize(64, 64);
        toolButton->setToolTip(t.name);
        QObject::connect(toolButton, &QPushButton::pressed, [toolButton, t]() {
            QMimeData* mimeData = new QMimeData;
            mimeData->setText(t.name);
            mimeData->setImageData(QPixmap(t.iconPath));

            QDrag* drag = new QDrag(toolButton);
            drag->setMimeData(mimeData);
            drag->setPixmap(QPixmap(t.iconPath).scaled(48, 48, Qt::KeepAspectRatio));
            drag->exec(Qt::CopyAction);
        });

        toolsLayout->addWidget(toolButton);
    }

    toolsWidget->setLayout(toolsLayout);
    toolsWidget->setFixedWidth(120);

    editorWidget = new MapCanvas(editorContainer);
    editorWidget->loadCityMap(cityName);
    editorLayout->addWidget(toolsWidget);
    editorLayout->addWidget(editorWidget, 1);
    editorContainer->setLayout(editorLayout);

    stackedWidget->addWidget(editorContainer);
    stackedWidget->setCurrentWidget(editorContainer);
}
