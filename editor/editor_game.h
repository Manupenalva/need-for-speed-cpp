#ifndef EDITOR_GAME_H
#define EDITOR_GAME_H

#include <QMainWindow>
#include <QToolButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class EditorGame;
}
QT_END_NAMESPACE

class EditorGame: public QMainWindow {
    Q_OBJECT

public:
    explicit EditorGame(QWidget* parent = nullptr);
    ~EditorGame() override;


private:
    Ui::EditorGame* ui;
    void setUpNav();
    void setUpLoad();
    void setUpTools();
    void dragMovement(QToolButton* btn, const QString& type, int rotDeg, const QString& iconPath);
    void rotateIcon(QToolButton* btn, const QString& iconPath, int rotDeg);
};
#endif
