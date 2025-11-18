#ifndef ACTION_RESULT_H
#define ACTION_RESULT_H

#include <QPointF>

#include "../drag_info.h"

struct ActionResult {
    bool success = false;
    bool selecting = false;
    DragInfo pending;
    QPointF pos;
    QString errorTitle = "";
    QString errorMessage = "";
};


#endif
