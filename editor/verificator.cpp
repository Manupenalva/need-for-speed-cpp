#include "verificator.h"

#include <QGraphicsItem>

#include "editor_constants.h"

Verificator::Verificator(const SceneController& controller, const QGraphicsScene& scene):
        controller(controller), scene(scene) {}

bool Verificator::validate(QString& errorTitle, QString& errorMessage) const {
    if (!checkStart(errorTitle, errorMessage)) {
        return false;
    }
    if (!checkFinish(errorTitle, errorMessage)) {
        return false;
    }
    if (!checkLine(errorTitle, errorMessage)) {
        return false;
    }
    if (!checkCheckpoint(errorTitle, errorMessage)) {
        return false;
    }
    return true;
}

bool Verificator::checkStart(QString& errorTitle, QString& errorMessage) const {
    int startCounting = controller.countItemsOfType(START_TYPE);
    if (startCounting != MAX_PLAYERS) {
        errorTitle = "Cars missing";
        errorMessage = "It is neccessary to be 8 cars points.";
        return false;
    }
    return true;
}

bool Verificator::checkFinish(QString& errorTitle, QString& errorMessage) const {
    int finishCounting = controller.countItemsOfType(FINISH_TYPE);
    if (finishCounting != MAX_FINISH) {
        errorTitle = "Finish missing";
        errorMessage = "It is neccessary to be 1 finish line.";
        return false;
    }
    return true;
}

bool Verificator::checkLine(QString& errorTitle, QString& errorMessage) const {
    int finishCounting = controller.countItemsOfType(LINE_TYPE);
    if (finishCounting != MAX_START_LINE) {
        errorTitle = "Starting line missing";
        errorMessage = "It is neccessary to be 1 start line.";
        return false;
    }
    return true;
}

bool Verificator::checkCheckpoint(QString& errorTitle, QString& errorMessage) const {
    int checkpointCounting = controller.countItemsOfType(CHECKPOINT_TYPE);
    if (checkpointCounting < MIN_CHECKPOINTS) {
        errorTitle = "Checkpoint Missing";
        errorMessage = "It is neccessary to be, al least, 2 checkpoints.";
        return false;
    }
    // if (!checkCheckpointsHints(errorTitle, errorMessage)){
    //     return false;
    // }
    return true;
}
