#ifndef CITYSELECTION_H
#define CITYSELECTION_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class CitySelection: public QWidget {
    Q_OBJECT

public:
    explicit CitySelection(QWidget* parent = nullptr);

signals:
    void citySelected(const QString& cityName);
};

#endif
