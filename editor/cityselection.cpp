#include "cityselection.h"

#include <QIcon>
#include <QPixmap>
#include <vector>
#include <QDir>

#define BACKGROUNDS "../client/assets/cities/"

CitySelection::CitySelection(QWidget* parent): QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("Select a City");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(20);

    auto* citiesLayout = new QHBoxLayout();
    QDir citiesDir(BACKGROUNDS);
    
    QStringList nameFilters;
    nameFilters << "*.png";

    QFileInfoList cityFiles = citiesDir.entryInfoList(nameFilters, QDir::Files);

    for (const QFileInfo& fileInfo : cityFiles) {
        QString filePath = fileInfo.absoluteFilePath();
        QString cityName = fileInfo.baseName();

        QVBoxLayout* cityLayout = new QVBoxLayout();
        QPushButton* cityButton = new QPushButton();
        cityButton->setIcon(QIcon(filePath));
        cityButton->setIconSize(QSize(200, 200));
        cityButton->setFixedSize(220, 220);
        QObject::connect(cityButton, &QPushButton::clicked, this,
                        [this, filePath]() { emit citySelected(filePath); });
        QLabel* nameLabel = new QLabel(cityName);
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
