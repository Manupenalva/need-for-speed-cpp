#include "lobby.h"

#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>

Lobby::Lobby(const char* hostname, const char* port, QWidget* parent): 
    QMainWindow(parent),
    protocol(hostname, port) {

    stack = new QStackedWidget(this);

    menu = new QWidget(this);
    auto* layoutMenu = new QVBoxLayout(menu);

    QLabel* title = new QLabel("Need for Speed - 2D");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: bold;");

    createGameButton = new QPushButton("New Game");
    joinGameButton = new QPushButton("Join Game");

    layoutMenu->addWidget(title);
    layoutMenu->addSpacing(20);
    layoutMenu->addWidget(createGameButton);
    layoutMenu->addWidget(joinGameButton);
    menu->setLayout(layoutMenu);

    connectScreen = new QWidget(this);
    auto* layoutConnection = new QVBoxLayout(connectScreen);

    QLabel* servnameLabel = new QLabel("Race code:");
    servname = new QLineEdit();

    connectButton = new QPushButton("Connect");
    backButton = new QPushButton("Back");

    layoutConnection->addWidget(servnameLabel);
    layoutConnection->addWidget(servname);
    layoutConnection->addWidget(connectButton);
    layoutConnection->addWidget(backButton);
    connectScreen->setLayout(layoutConnection);

    stack->addWidget(menu);
    stack->addWidget(connectScreen);
    setCentralWidget(stack);

    connect(createGameButton, &QPushButton::clicked, this, &Lobby::createGame);
    connect(joinGameButton, &QPushButton::clicked, this, &Lobby::showConnectScreen);
    connect(connectButton, &QPushButton::clicked, this, &Lobby::connectServer);
    connect(backButton, &QPushButton::clicked, this, &Lobby::menuScreen);

    setWindowTitle("Need for Speed 2D - Lobby");
    stack->setCurrentIndex(0);
}

void Lobby::menuScreen() { stack->setCurrentIndex(0); }

void Lobby::showConnectScreen() { stack->setCurrentIndex(1); }

void Lobby::createGame() {
    ClientMessageDTO msg;
    msg.type = MsgType::CREATE_RACE;
    protocol.send_client_message(msg);

    ServerMessageDTO response = protocol.recv_server_message();
    if (response.type != MsgType::SEND_CLIENT_ID) {
        QMessageBox::warning(this, "Error", "Failed to create a new game.");
        return;
    }
    QMessageBox::information(this, "New Game",
                            QString("Race code: %1").arg(response.id));
    this->close();
}

void Lobby::connectServer() {
    QString raceCode = servname->text();

    if (raceCode.isEmpty()) {
        QMessageBox::warning(this, "Error", "You must insert text a race code");
        return;
    }

    ClientMessageDTO msg;
    msg.type = MsgType::JOIN_RACE;
    msg.lobby_id = raceCode.toUshort();
    protocol.send_client_message(msg);
    ServerMessageDTO response = protocol.recv_server_message();

    if (response.type != MsgType::JOIN_RESULT) {
        QMessageBox::warning(this, "Error", "Failed to join the game.");
        return;
    }

    if (!response.joined) {
        QMessageBox::warning(this, "Error", "Could not join the game. It may be full or nonexistent.");
        return;
    }

    QMessageBox::information(
            this, "Connecting",
            QString("Connectin to.. %1")
                    .arg(raceCode));
    this->close(); 
}