#ifndef LOBBY_H
#define LOBBY_H

#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>
#include <QSoundEffect>
#include <string>

#include "../../common/protocol.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Lobby;
}
QT_END_NAMESPACE

class Lobby: public QMainWindow {
    Q_OBJECT

public:
    explicit Lobby(Protocol& protocol, QWidget* parent = nullptr);
    ~Lobby();

    // cppcheck-suppress unknownMacro
private slots:
    void menuScreen();
    void showConnectScreen();
    void connectServer();
    void createGame();
    void startGame();
    void updateLobby();
    void showCarSelection();
    void showCatalog();
    void confirmCar();

private:
    Ui::Lobby* ui;
    Protocol& protocol;

    QStackedWidget* stack = nullptr;
    QTimer* timer = nullptr;
    QSoundEffect* musicEffect = nullptr;

    bool host;
    int raceC;
    uint16_t chosenCar = 0;
};

#endif
