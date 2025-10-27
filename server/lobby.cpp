#include "lobby.h"

#include <string>
#include <utility>

#include "events/joinlobbymessage.h"

Lobby::Lobby(std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> lobbyQueue,
             MonitorClients& clientsMonitor):
        lobby_queue(std::move(lobbyQueue)), clients_monitor(clientsMonitor) {}

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

int Lobby::create_race() { return clients_monitor.create_race(); }

void Lobby::add_player_to_race(int playerId, int raceId) {
    clients_monitor.add_client_to_race(playerId, raceId);
}

void Lobby::remove_player_from_race(int playerId) {
    clients_monitor.remove_client_from_race(playerId);
}

void Lobby::start_race(int playerId) {
    int race_id = clients_monitor.get_player_race(playerId);
    if (race_id == -1) {
        return;
    }
    auto session = std::make_shared<GameSession>(
            race_id, clients_monitor);  // Evito que se llame al destructor
    active_games.push_back(session);
}

void Lobby::manage_msg(std::shared_ptr<ClientHandlerMessage> msg) {
    MsgType type = msg->get_msg_type();
    int client_id = msg->get_client_id();

    switch (type) {
        case MsgType::CREATE_RACE: {
            clean_games();  // Limpio juegos terminados antes de crear uno nuevo
            int race_id = create_race();
            add_player_to_race(client_id, race_id);
            auto client = clients_monitor.get_client(client_id);
            // Envio codigo de partida al cliente
            if (client) {
                ServerMessageDTO response;
                response.type = MsgType::SEND_CLIENT_ID;
                response.id = race_id;
                client->send_msg(response);
            }
            break;
        }
        case MsgType::JOIN_RACE: {
            const auto* joinMsg = dynamic_cast<JoinLobbyMessage*>(msg.get());
            if (!joinMsg) {
                break;
            }
            int lobby_code = joinMsg->get_race_id();
            bool success = clients_monitor.add_client_to_race(client_id, lobby_code);
            auto client = clients_monitor.get_client(client_id);
            if (client) {
                ServerMessageDTO response;
                response.type = MsgType::JOIN_RESULT;
                response.joined = success;
                client->send_msg(response);
            }
            // recibe la race id asi no esta hardcodeado
            // add_player_to_race(client_id, race_id);  // hardcodeado
            break;
        }
        case MsgType::EXIT_RACE: {
            remove_player_from_race(client_id);
            break;
        }
        case MsgType::START_RACE: {
            start_race(msg->get_race_id());
            break;
        }
        case MsgType::GET_LOBBIES: {
            // send_lobbies_info(client_id);
            break;
        }
        default: {
        }
    }
}

void Lobby::clean_games() {
    active_games.erase(std::remove_if(active_games.begin(), active_games.end(),
                                      [](const std::shared_ptr<GameSession>& game) {
                                          return !game->is_running();
                                      }),
                       active_games.end());
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
