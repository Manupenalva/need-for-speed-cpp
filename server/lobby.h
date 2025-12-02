#ifndef LOBBY_H
#define LOBBY_H

#include <memory>
#include <string>
#include <vector>

#include "../common/constants.h"
#include "../common/lobbyinfo.h"
#include "../common/queue.h"
#include "../common/thread.h"
#include "events/clienthandlermessage.h"
#include "monitors/monitorClients.h"
#include "monitors/monitorGames.h"
#include "race_logic/carBuilder.h"

#include "configLoader.h"
#include "gameSession.h"

class Lobby: public Thread {

public:
    Lobby(std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> lobbyQueue,
          MonitorClients& clientsMonitor);
    ~Lobby();

    void run() override;
    void shutdown();

private:
    std::vector<std::shared_ptr<GameSession>> active_games;
    std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> lobby_queue;
    MonitorClients& clients_monitor;
    MonitorGames games_monitor;
    std::vector<CarProperties> car_catalog;
    std::shared_ptr<ConfigConstants> config;
    int create_race();
    void add_player_to_race(int playerId, int raceId);
    void remove_player_from_race(int playerId);
    void start_race(int raceId);
    void manage_msg(std::shared_ptr<ClientHandlerMessage> msg);
    void clean_games();
    void handle_create_race(int client_id);
    void handle_join_race(const std::shared_ptr<ClientHandlerMessage>& msg, int client_id);
    void handle_lobby_update(int client_id);
    void handle_get_car_catalog(int client_id);
    void handle_new_username(const std::shared_ptr<ClientHandlerMessage>& msg, int client_id);
};
#endif
