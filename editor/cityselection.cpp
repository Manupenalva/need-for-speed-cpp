#include "cityselection.h"

#include <QIcon>
#include <QPixmap>
#include <vector>

CitySelection::CitySelection(QWidget* parent): QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("Select a City");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(20);

    auto* citiesLayout = new QHBoxLayout();

    struct City {
        QString name;
        QString iconPath;
    };

    std::vector<City> cities = {
            {"Liberty City", "/home/facu/Escritorio/tps/tp_grupal/tp-taller-g4/client/assets/"
                             "cities/Backgrounds - Liberty City.png"},
            {"San Andreas", "/home/facu/Escritorio/tps/tp_grupal/tp-taller-g4/client/assets/cities/"
                            "Backgrounds - San Andreas.png"},
            {"Vice City", "/home/facu/Escritorio/tps/tp_grupal/tp-taller-g4/client/assets/cities/"
                          "Backgrounds - Vice City.png"}};

    for (auto& city: cities) {
        QVBoxLayout* cityLayout = new QVBoxLayout();
        QPushButton* cityButton = new QPushButton();
        cityButton->setIcon(QIcon(city.iconPath));
        cityButton->setIconSize(QSize(200, 200));
        cityButton->setFixedSize(220, 220);
        QObject::connect(cityButton, &QPushButton::clicked, this,
                        [this, city]() { emit citySelected(city.iconPath); });
        QLabel* nameLabel = new QLabel(city.name);
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
        cityLayout->addWidget(cityButton);
        cityLayout->addWidget(nameLabel);
        citiesLayout->addLayout(cityLayout);
    }

    mainLayout->addLayout(citiesLayout);
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    setLayout(mainLayout);
}
