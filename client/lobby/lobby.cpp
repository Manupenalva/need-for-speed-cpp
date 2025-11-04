#include "lobby.h"

#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>

Lobby::Lobby(Protocol& protocol, QWidget* parent): QMainWindow(parent), protocol(protocol) {

    stack = new QStackedWidget(this);

    menu = new QWidget(this);
    auto* layoutMenu = new QVBoxLayout(menu);
    timer = new QTimer(this);

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

    startScreen = new QWidget(this);
    auto* startLayout = new QVBoxLayout(startScreen);
    QLabel* startTitle = new QLabel("Lobby Start");
    startTitle->setAlignment(Qt::AlignCenter);
    startTitle->setStyleSheet("font-size: 24px; font-weight: bold;");
    startButton = new QPushButton("Start");
    startButton->setEnabled(false);
    playerLabel = new QLabel("Players: 1 / ?");
    codeLabel = new QLabel("Game code: ?");
    playerLabel->setAlignment(Qt::AlignCenter);
    playerLabel->setStyleSheet("font-size: 16px;");
    codeLabel->setAlignment(Qt::AlignCenter);
    codeLabel->setStyleSheet("font-size: 16px;");

    startLayout->addWidget(startTitle);
    startLayout->addWidget(playerLabel);
    startLayout->addWidget(codeLabel);
    startLayout->addWidget(startButton);
    startLayout->addWidget(backButton);
    startScreen->setLayout(startLayout);

    stack->addWidget(menu);
    stack->addWidget(connectScreen);
    stack->addWidget(startScreen);
    setCentralWidget(stack);

    connect(createGameButton, &QPushButton::clicked, this, &Lobby::createGame);
    connect(joinGameButton, &QPushButton::clicked, this, &Lobby::showConnectScreen);
    connect(connectButton, &QPushButton::clicked, this, &Lobby::connectServer);
    connect(backButton, &QPushButton::clicked, this, &Lobby::menuScreen);
    connect(startButton, &QPushButton::clicked, this, &Lobby::startGame);
    connect(timer, &QTimer::timeout, this, &Lobby::updateLobby);

    setWindowTitle("Need for Speed 2D - Lobby");
    resize(400, 300);
    stack->setCurrentIndex(0);
}

void Lobby::menuScreen() {
    stack->setCurrentIndex(0);
    ClientMessageDTO msg;
    msg.type = MsgType::EXIT_RACE;
    protocol.send_client_message(msg);
    timer->stop();
}

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
    host = true;
    QMessageBox::information(this, "New Game", QString("Race code: %1").arg(response.id));
    raceC = response.id;
    codeLabel->setText(QString("Game Code: %1").arg(raceC));
    stack->setCurrentIndex(2);
    timer->start(1000);
}

void Lobby::connectServer() {
    QString raceCode = servname->text();

    if (raceCode.isEmpty()) {
        QMessageBox::warning(this, "Error", "You must insert text a race code");
        return;
    }

    ClientMessageDTO msg;
    msg.type = MsgType::JOIN_RACE;
    bool ok;
    msg.lobby_id = raceCode.toUShort(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Error", "Invalid race code");
        return;
    }
    protocol.send_client_message(msg);
    ServerMessageDTO response = protocol.recv_server_message();

    if (response.type != MsgType::JOIN_RESULT) {
        QMessageBox::warning(this, "Error", "Failed to join the game.");
        return;
    }

    if (!response.joined) {
        QMessageBox::warning(this, "Error",
                             "Could not join the game. It may be full or nonexistent.");
        return;
    }
    host = false;
    raceC = msg.lobby_id;
    QMessageBox::information(this, "Connecting", QString("Connectin to.. %1").arg(raceCode));
    codeLabel->setText(QString("Game Code: %1").arg(raceC));
    stack->setCurrentIndex(2);
    timer->start(1000);  // Actualizar cada segundo
}

void Lobby::startGame() {
    if (!host) {
        QMessageBox::warning(this, "Error", "Could not create a game because you are not the host");
        return;
    }
    ClientMessageDTO msg;
    msg.type = MsgType::START_RACE;
    msg.lobby_id = raceC;
    protocol.send_client_message(msg);
    timer->stop();
    this->close();
}

void Lobby::updateLobby() {
    ClientMessageDTO msg;
    msg.type = MsgType::GET_LOBBY_UPDATE;
    protocol.send_client_message(msg);
    ServerMessageDTO response = protocol.recv_server_message();
    if (response.type == MsgType::SEND_LOBBY_UPDATE) {
        auto& lobbyInfo = response.lobby_info;
        int count = static_cast<int>(lobbyInfo.player_amount);
        int max = static_cast<int>(lobbyInfo.max_players);
        playerLabel->setText(QString("Players: %1 / %2").arg(count).arg(max));
        startButton->setEnabled(host && count >= 2);
    } else if (response.type == MsgType::GAME_START) {
        timer->stop();
        this->close();
    }
}
