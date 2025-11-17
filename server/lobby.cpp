#include "lobby.h"

#include <string>
#include <utility>

#include "events/joinlobbymessage.h"

Lobby::Lobby(std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> lobbyQueue,
             MonitorClients& clientsMonitor):
        lobby_queue(std::move(lobbyQueue)),
        clients_monitor(clientsMonitor),
        car_catalog(CarBuilder("../server/assets/cars_configs/cars_config.yaml").get_catalog()) {}

void Lobby::run() {
    std::shared_ptr<ClientHandlerMessage> msg;

    while (should_keep_running()) {
        try {
            msg = lobby_queue->pop();
        } catch (const ClosedQueue& e) {
            return;
        }
        manage_msg(msg);
    }
}

void Lobby::handle_create_race(int client_id) {
    int race_id = create_race();
    auto client = clients_monitor.get_client(client_id);
    if (!client) {
        return;
    }
    games_monitor.insert_client_to_race(race_id, client);
    client->set_race_id(race_id);
    // Envio codigo de partida al cliente
    ServerMessageDTO response;
    response.type = MsgType::SEND_CLIENT_ID;
    response.id = race_id;
    client->send_msg(response);
}

void Lobby::handle_join_race(const std::shared_ptr<ClientHandlerMessage>& msg, int client_id) {
    const auto* joinMsg = dynamic_cast<const JoinLobbyMessage*>(msg.get());
    if (!joinMsg) {
        return;
    }
    int lobby_code = joinMsg->get_race_id();
    add_player_to_race(client_id, lobby_code);
}

void Lobby::handle_lobby_update(int client_id) {
    auto client = clients_monitor.get_client(client_id);
    if (!client) {
        return;
    }
    LobbyInfo lobby_info;
    int race_id = client->get_race_id();
    std::shared_ptr<RaceStruct> race = games_monitor.get_race(race_id);
    lobby_info.lobby_id = static_cast<uint16_t>(race_id);
    lobby_info.player_amount = race->size();
    lobby_info.max_players = MAX_PLAYERS_RACE;
    ServerMessageDTO response;
    response.type = MsgType::SEND_LOBBY_UPDATE;
    response.lobby_info = lobby_info;
    client->send_msg(response);
}

void Lobby::handle_get_car_catalog(int client_id) {
    auto client = clients_monitor.get_client(client_id);
    if (!client) {
        return;
    }
    ServerMessageDTO response;
    response.type = MsgType::SEND_CAR_CATALOG;
    response.car_catalog = car_catalog;
    client->send_msg(response);
}

int Lobby::create_race() { return games_monitor.create_race(); }

void Lobby::add_player_to_race(int playerId, int raceId) {
    auto client = clients_monitor.get_client(playerId);
    if (!client) {
        return;
    }
    bool result = games_monitor.insert_client_to_race(raceId, client);
    ServerMessageDTO response;
    response.type = MsgType::JOIN_RESULT;
    if (!result) {
        response.joined = false;
    } else {
        client->set_race_id(raceId);
        response.joined = true;
    }
    client->send_msg(response);
}


void Lobby::remove_player_from_race(int playerId) {
    auto client = clients_monitor.get_client(playerId);
    if (!client) {
        return;
    }
    int race_id = client->get_race_id();
    games_monitor.remove_client_from_race(race_id, playerId);
    client->set_race_id(-1);
}

void Lobby::start_race(int playerId) {
    auto client = clients_monitor.get_client(playerId);
    if (!client) {
        return;
    }
    int race_id = client->get_race_id();
    if (race_id == -1) {
        return;
    }
    auto session = std::make_shared<GameSession>(
            race_id, games_monitor.get_race(race_id));  // Evito que se llame al destructor
    active_games.push_back(session);
}

void Lobby::manage_msg(std::shared_ptr<ClientHandlerMessage> msg) {
    MsgType type = msg->get_msg_type();
    int client_id = msg->get_client_id();

    switch (type) {
        case MsgType::CREATE_RACE: {
            handle_create_race(client_id);
            break;
        }
        case MsgType::JOIN_RACE: {
            handle_join_race(msg, client_id);
            break;
        }
        case MsgType::EXIT_RACE: {
            remove_player_from_race(client_id);
            break;
        }
        case MsgType::START_RACE: {
            clean_games();  // Limpio juegos terminados antes de crear uno nuevo
            start_race(client_id);
            break;
        }
        case MsgType::GET_LOBBIES: {
            // send_lobbies_info(client_id);
            break;
        }
        case MsgType::GET_LOBBY_UPDATE: {
            handle_lobby_update(client_id);
            break;
        }
        case MsgType::GET_CAR_CATALOG: {
            handle_get_car_catalog(client_id);
            break;
        }
        default: {
        }
    }
}

void Lobby::clean_games() {

    for (auto it = active_games.begin(); it != active_games.end();) {
        if (!(*it)->is_running()) {
            games_monitor.remove_race((*it)->get_id());
            it = active_games.erase(it);
        } else {
            ++it;
        }
    }
}

void Lobby::shutdown() {
    stop();
    try {
        lobby_queue->close();
    } catch (const std::runtime_error& e) {
        return;
    }
}

Lobby::~Lobby() {
    shutdown();
    this->join();
}
