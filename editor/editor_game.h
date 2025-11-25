#ifndef EDITOR_GAME_H
#define EDITOR_GAME_H

#include <QMainWindow>
#include <QToolButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class EditorGame;
}
QT_END_NAMESPACE

struct Tools {
    QToolButton* btn;
    QString type;
    QString iconPath;
    int rotation;
};

class EditorGame: public QMainWindow {
    Q_OBJECT

public:
    explicit EditorGame(QWidget* parent = nullptr);
    ~EditorGame() override;


private:
    Ui::EditorGame* ui;
    std::vector<Tools> tools;
    void setUpNav();
    void setUpLoad();
    void setUpTools();
    void dragMovement(QToolButton* btn, const QString& type, const QString& iconPath,
                      int rotDeg = 0);
    void rotateIcon(QToolButton* btn, const QString& iconPath, int rotDeg);
    void toolsConstants();
};
#endif
