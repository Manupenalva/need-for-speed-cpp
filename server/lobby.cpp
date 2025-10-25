#include "lobby.h"

#include <utility>

Lobby::Lobby(std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> lobbyQueue,
             MonitorClients& clientsMonitor):
        lobby_queue(std::move(lobbyQueue)), clients_monitor(clientsMonitor) {}

void Lobby::start_lobby() {
    std::shared_ptr<ClientHandlerMessage> msg;
    try {
        msg = lobby_queue->pop();
    } catch (const ClosedQueue& e) {
        return;
    }
    manage_msg(msg);
}

int Lobby::create_race() { return clients_monitor.create_race(); }

void Lobby::add_player_to_race(int playerId, int raceId) {
    clients_monitor.add_client_to_race(playerId, raceId);
}

void Lobby::remove_player_from_race(int playerId) {
    clients_monitor.remove_client_from_race(playerId);
}

void Lobby::start_race(int raceId) {
    auto session = std::make_shared<GameSession>(
            raceId, clients_monitor);  // Evito que se llame al destructor
    active_games.push_back(session);
}

void Lobby::manage_msg(std::shared_ptr<ClientHandlerMessage> msg) {
    MsgType type = msg->get_msg_type();
    int client_id = msg->get_client_id();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
    switch (type) {
        case MsgType::CREATE_RACE: {
            clean_games();  // Limpio juegos terminados antes de crear uno nuevo
            int race_id = create_race();
            add_player_to_race(client_id, race_id);
            break;
        }
        case MsgType::JOIN_RACE: {
            add_player_to_race(client_id, 0);  // hardcodeado
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
            break;
        }
    }
#pragma GCC diagnostic pop
}

void Lobby::clean_games() {
    active_games.erase(std::remove_if(active_games.begin(), active_games.end(),
                                      [](const std::shared_ptr<GameSession>& game) {
                                          return !game->is_running();
                                      }),
                       active_games.end());
}
