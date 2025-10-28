#ifndef LOBBY_H
#define LOBBY_H

#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <string>

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

private:
    QStackedWidget* stack;
    QWidget* menu;
    QPushButton* createGameButton;
    QPushButton* joinGameButton;

    QWidget* connectScreen;
    QLineEdit* servname;
    QPushButton* connectButton;
    QPushButton* backButton;
};

#endif
