#include "lobby.h"

#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>

#include "ui_lobby.h"

Lobby::Lobby(Protocol& protocol, QWidget* parent):
        QMainWindow(parent), ui(new Ui::Lobby), protocol(protocol) {

    ui->setupUi(this);

    stack = ui->stackedWidget;
    timer = new QTimer(this);

    ui->carListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->startButton->setEnabled(false);

    connect(ui->createGameButton, &QPushButton::clicked, this, &Lobby::createGame);
    connect(ui->joinGameButton, &QPushButton::clicked, this, &Lobby::showConnectScreen);
    connect(ui->connectButton, &QPushButton::clicked, this, &Lobby::connectServer);

    connect(ui->backConnectButton, &QPushButton::clicked, this, &Lobby::menuScreen);
    connect(ui->backCarButton, &QPushButton::clicked, this, &Lobby::menuScreen);
    connect(ui->backStartButton, &QPushButton::clicked, this, &Lobby::menuScreen);

    connect(ui->startButton, &QPushButton::clicked, this, &Lobby::startGame);
    connect(ui->carListWidget, &QListWidget::currentRowChanged, this, [this](int row) {
        if (row < 0) {
            ui->carPreviewLabel->setText("Select a car to see details");
            return;
        }
        auto* it = ui->carListWidget->item(row);
        ui->carPreviewLabel->setText(it ? it->toolTip() : "");
    });
    connect(ui->confirmButton, &QPushButton::clicked, this, &Lobby::confirmCar);
    connect(timer, &QTimer::timeout, this, &Lobby::updateLobby);

    setWindowTitle("Need for Speed 2D - Lobby");
    resize(600, 500);
    stack->setCurrentIndex(0);

    audioOutput = new QAudioOutput(this);
    musicPlayer = new QMediaPlayer(this);
    musicPlayer->setAudioOutput(audioOutput);

    QString musicPath = "../client/resources/sounds/Need For Speed Music.mp3";
    musicPlayer->setSource(QUrl::fromLocalFile(musicPath));

    audioOutput->setVolume(0.5);
    musicPlayer->setLoops(QMediaPlayer::Infinite);
    musicPlayer->play();
}

Lobby::~Lobby() { 
    if (musicPlayer){
        musicPlayer->stop();
    }
    delete ui; 
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
    ui->gameCodeLabel->setText(QString("Game Code: %1").arg(raceC));
    timer->start(1000);
    showCarSelection();
}

void Lobby::connectServer() {
    QString raceCode = ui->raceCodeLineEdit->text();

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
    ui->gameCodeLabel->setText(QString("Game Code: %1").arg(raceC));
    timer->start(1000);
    showCarSelection();
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
        ui->playersLabel->setText(QString("Players: %1 / %2").arg(count).arg(max));
        ui->startButton->setEnabled(host && count >= 2);
    } else if (response.type == MsgType::GAME_START) {
        timer->stop();
        ClientMessageDTO msg;
        msg.type = MsgType::SELECT_CAR;
        msg.car_id = chosenCar;
        protocol.send_client_message(msg);
        musicPlayer->stop();
        this->close();
    }
}

void Lobby::showCarSelection() {
    stack->setCurrentIndex(2);
    showCatalog();
}

void Lobby::showCatalog() {
    ClientMessageDTO req;
    req.type = MsgType::GET_CAR_CATALOG;
    protocol.send_client_message(req);

    ServerMessageDTO resp = protocol.recv_server_message();

    if (resp.type != MsgType::SEND_CAR_CATALOG) {
        QMessageBox::warning(this, "Error", "Could not get car catalog.");
        return;
    }

    ui->carListWidget->clear();

    for (const auto& car: resp.car_catalog) {
        auto* item = new QListWidgetItem(QString("Car %1").arg(car.car_id));
        item->setData(Qt::UserRole, static_cast<int>(car.car_id));
        item->setToolTip(
                QString("Max speed: %1\nAcceleration: %2\nHealth: %3\nMass: %4\nControl: %5")
                        .arg(car.max_speed)
                        .arg(car.acceleration)
                        .arg(car.max_health)
                        .arg(car.mass)
                        .arg(car.control));
        ui->carListWidget->addItem(item);
    }

    if (ui->carListWidget->count() > 0)
        ui->carListWidget->setCurrentRow(0);
}

void Lobby::confirmCar() {
    auto* item = ui->carListWidget->currentItem();
    if (!item) {
        QMessageBox::information(this, "Select car", "Please choose a car.");
        return;
    }
    chosenCar = static_cast<uint16_t>(item->data(Qt::UserRole).toInt());
    stack->setCurrentIndex(3);
}
