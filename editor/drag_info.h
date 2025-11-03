#ifndef DRAG_INFO_H
#define DRAG_INFO_H

#include <QString>
#include <QByteArray>

class DragInfo {
private:
    QString type;
    int rotation;
    QString iconPath;
public:
    DragInfo() = default;
    DragInfo(const QString& type, int rotation, const QString& iconPath);
    
    QByteArray pack() const;
    bool unpack(const QByteArray& data);
    QString mimeType() const;
    QString getType() const;
    int getRotation() const;
    QString getIconPath() const;
};
#endif