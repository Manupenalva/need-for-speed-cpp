#ifndef VERIFICATOR_H
#define VERIFICATOR_H

#include <QString>
#include <QGraphicsScene>
#include "scene_controller.h"

class Verificator {
public:
    Verificator(const SceneController& controller, const QGraphicsScene& scene);

    bool validate(QString& errorTitle, QString& errorMessage) const;
private:
    const SceneController& controller;
    const QGraphicsScene& scene;

    bool checkStart(QString& errorTitle, QString& errorMessage) const;
    bool checkFinish(QString& errorTitle, QString& errorMessage) const;
    bool checkLine(QString& errorTitle, QString& errorMessage) const;

    bool checkCheckpoint(QString& errorTitle, QString& errorMessage) const;
    //bool checkCheckpointsHints(QString& errorTitle, QString& errorMessage) const;
};

#endif
