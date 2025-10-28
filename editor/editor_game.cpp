#include "editor_game.h"

#include <QLabel>
#include <QVBoxLayout>

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
    editorWidget->loadCityMap(cityName);
    stackedWidget->setCurrentWidget(editorWidget);
}
