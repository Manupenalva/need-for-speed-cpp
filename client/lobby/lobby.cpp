#include "lobby.h"

#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>
#include <utility>

#include "ui_lobby.h"

Lobby::Lobby(Protocol& protocol, bool& started, QWidget* parent):
        QMainWindow(parent), ui(new Ui::Lobby), protocol(protocol), started(started) {

    ui->setupUi(this);

    stack = ui->stackedWidget;
    timer = new QTimer(this);

    ui->startButton->setEnabled(false);

    connect(ui->createGameButton, &QPushButton::clicked, this, &Lobby::createGame);
    connect(ui->joinGameButton, &QPushButton::clicked, this, &Lobby::showConnectScreen);
    connect(ui->connectButton, &QPushButton::clicked, this, &Lobby::connectServer);

    connect(ui->backConnectButton, &QPushButton::clicked, this, &Lobby::menuScreen);
    connect(ui->backCarButton, &QPushButton::clicked, this, &Lobby::menuScreen);
    connect(ui->backStartButton, &QPushButton::clicked, this, &Lobby::menuScreen);

    connect(ui->startButton, &QPushButton::clicked, this, &Lobby::startGame);
    connect(ui->nextCarButton, &QPushButton::clicked, this, &Lobby::nextCar);
    connect(ui->prevCarButton, &QPushButton::clicked, this, &Lobby::prevCar);

    connect(ui->confirmButton, &QPushButton::clicked, this, &Lobby::confirmCar);
    connect(timer, &QTimer::timeout, this, &Lobby::updateLobby);

    setWindowTitle("Need for Speed 2D - Lobby");
    resize(600, 500);
    stack->setCurrentIndex(0);

    ui->carImageLabel->setScaledContents(true);

    musicEffect = new QSoundEffect(this);
    musicEffect->setSource(QUrl::fromLocalFile("../client/resources/sounds/need_for_speed.wav"));
    musicEffect->setLoopCount(QSoundEffect::Infinite);
    musicEffect->setVolume(0.2f);
    musicEffect->play();
}

Lobby::~Lobby() {
    if (musicEffect) {
        musicEffect->stop();
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

void Lobby::showConnectScreen() {
    if (!sendUsername()) {
        return;
    }
    stack->setCurrentIndex(1);
}

void Lobby::createGame() {
    if (!sendUsername()) {
        return;
    }
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

    if (!response.result) {
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
        ui->startButton->setEnabled(host);
    } else if (response.type == MsgType::GAME_START) {
        timer->stop();
        started = true;
        ClientMessageDTO msg;
        msg.type = MsgType::SELECT_CAR;
        msg.car_id = chosenCar;
        protocol.send_client_message(msg);
        musicEffect->stop();
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

    cars.clear();
    currentIndex = 0;

    for (const auto& car: resp.car_catalog) {
        uiCar gcar;
        gcar.id = static_cast<int>(car.car_id);
        gcar.info = QString("Max speed: %1\nAcceleration: %2\nHealth: %3\nMass: %4\nControl: %5")
                            .arg(car.max_speed)
                            .arg(car.acceleration)
                            .arg(car.max_health)
                            .arg(car.mass)
                            .arg(car.control);
        cars.push_back(std::move(gcar));
    }

    updateCarCarousel();
}

void Lobby::confirmCar() {
    if (cars.empty()) {
        QMessageBox::information(this, "Select car", "Please choose a car.");
        return;
    }
    chosenCar = cars[currentIndex].id;
    stack->setCurrentIndex(3);
}

bool Lobby::sendUsername() {
    QString username = ui->playerNameLineEdit->text().trimmed();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "Error", "You must insert a username");
        return false;
    }

    ClientMessageDTO msg;
    msg.type = MsgType::SEND_NAME;
    msg.name = username.toStdString();
    protocol.send_client_message(msg);

    ServerMessageDTO response = protocol.recv_server_message();
    if (response.type == MsgType::NAME_RESULT) {
        if (response.result) {
            return true;
        }
        QMessageBox::warning(this, "Error", "This username already exists");
        return false;
    }
    return false;
}

void Lobby::updateCarCarousel() {
    if (currentIndex < 0 || currentIndex >= static_cast<int>(cars.size())) {
        currentIndex = 0;
    }
    uiCar& car = cars[currentIndex];
    ui->carInfoLabel->setText(car.info);

    QString img = QString("../client/resources/assets/lobby/car%1.png").arg(car.id);

    QPixmap pix(img);
    QPixmap scaled = pix.scaled(ui->carImageLabel->width(), ui->carImageLabel->height(),
                                Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->carImageLabel->setPixmap(scaled);
}

void Lobby::nextCar() {
    currentIndex = (currentIndex + 1) % static_cast<int>(cars.size());
    updateCarCarousel();
}

void Lobby::prevCar() {
    currentIndex =
            ((currentIndex - 1) + static_cast<int>(cars.size())) % static_cast<int>(cars.size());
    updateCarCarousel();
}
