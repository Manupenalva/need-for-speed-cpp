#ifndef EDITOR_GAME_H
#define EDITOR_GAME_H

#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>

#include "cityselection.h"
#include "mapcanvas.h"

class EditorGame: public QMainWindow {
    Q_OBJECT

public:
    explicit EditorGame(QWidget* parent = nullptr);
    ~EditorGame();

    // cppcheck-suppress unknownMacro
private slots:
    void goToCitySelection();
    void operEditorWithCity(const QString& cityName);

private:
    QStackedWidget* stackedWidget;
    QWidget* mainMenuWidget;
    CitySelection* citySelectionWidget;
    MapCanvas* editorWidget;
};
#endif
