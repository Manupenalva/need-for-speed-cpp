#ifndef LOBBY_H
#define LOBBY_H

#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>
#include <string>
#include <QLabel>

#include "../../common/protocol.h"

class Lobby: public QMainWindow {
    Q_OBJECT
    Protocol& protocol;

public:
    explicit Lobby(Protocol& protocol, QWidget* parent = nullptr);

    // cppcheck-suppress unknownMacro
private slots:
    void menuScreen();
    void showConnectScreen();
    void connectServer();
    void createGame();
    void startGame();
    void updateLobby();

private:
    QStackedWidget* stack;
    QWidget* menu;
    QPushButton* createGameButton;
    QPushButton* joinGameButton;
    QTimer* timer;

    QWidget* connectScreen;
    QLineEdit* servname;
    QPushButton* connectButton;
    QPushButton* backButton;
    QPushButton* startButton;
    QWidget* startScreen;
    QLabel* playerLabel;
    QLabel* codeLabel;
    bool host;
    int raceC;
};

#endif
