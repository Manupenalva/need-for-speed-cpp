#ifndef LOBBY_H
#define LOBBY_H

#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QPixmap>
#include <QPushButton>
#include <QSoundEffect>
#include <QStackedWidget>
#include <QTimer>
#include <string>
#include <vector>

#include "../../common/protocol.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Lobby;
}
QT_END_NAMESPACE

struct uiCar {
    uint16_t id;
    QString info;
};

class Lobby: public QMainWindow {
    Q_OBJECT

public:
    explicit Lobby(Protocol& protocol, bool& started, QWidget* parent = nullptr);
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
    bool& started;

    QStackedWidget* stack = nullptr;
    QTimer* timer = nullptr;
    QSoundEffect* musicEffect = nullptr;

    bool host;
    int raceC;
    uint16_t chosenCar = 0;
    std::vector<uiCar> cars;
    int currentIndex = 0;

    bool sendUsername();
    void updateCarCarousel();
    void nextCar();
    void prevCar();
};

#endif
