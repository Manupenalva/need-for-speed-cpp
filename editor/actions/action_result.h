#ifndef ACTION_RESULT_H
#define ACTION_RESULT_H

#include "../drag_info.h"
#include <QPointF>

struct ActionResult
{
    bool success = false;
    bool selecting = false;
    DragInfo pending;
    QPointF pos;
    QString errorTitle = "";
    QString errorMessage = "";
};


#endif