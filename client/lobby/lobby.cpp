#include "lobby.h"

#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>

Lobby::Lobby(QWidget* parent): QMainWindow(parent) {

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

    QLabel* servnameLabel = new QLabel("Host / IP:");
    servname = new QLineEdit();
    QLabel* portLabel = new QLabel("Port:");
    port = new QLineEdit();

    connectButton = new QPushButton("Connect");
    backButton = new QPushButton("Back");

    layoutConnection->addWidget(servnameLabel);
    layoutConnection->addWidget(servname);
    layoutConnection->addWidget(portLabel);
    layoutConnection->addWidget(port);
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
    resize(400, 300);
    stack->setCurrentIndex(0);
}

void Lobby::menuScreen() { stack->setCurrentIndex(0); }

void Lobby::showConnectScreen() { stack->setCurrentIndex(1); }

void Lobby::createGame() {
    QMessageBox::information(this, "New Game",
                             "Nuevo juego");  // Aca deberia mandarle al servidor que quiere crear
                                              // partida y mandar a pantalla de SDL
}

void Lobby::connectServer() {
    QString host = servname->text();
    QString p = port->text();

    if (host.isEmpty() || p.isEmpty()) {
        QMessageBox::warning(this, "Error", "You must insert text (host and port)");
        return;
    }
    QMessageBox::information(
            this, "Connecting",
            QString("Connectin to.. %1:%2")
                    .arg(host, p));  // Aca deberia mandarle al servidor mensaje de unirse y a
                                     // quien; y mandar a pantalla de SDL
}

/*
En client deberia aparacer
QApplication app(argc, argv)
Lobby lobby

lobby.show()
app.exec()
*/
