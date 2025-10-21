#ifndef LOBBY_H
#define LOBBY_H

#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>

class Lobby: public QMainWindow {
    Q_OBJECT

public:
    explicit Lobby(QWidget* parent = nullptr);

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
    QLineEdit* port;
    QPushButton* connectButton;
    QPushButton* backButton;
};

#endif
