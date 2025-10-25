#ifndef MONITORCLIENTS_H
#define MONITORCLIENTS_H

#include <algorithm>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "../common/messageDTOs.h"

#include "client_handler.h"
#include "monitorGames.h"
#include "racemonitor.h"

class Client;

class MonitorClients {
private:
    std::unordered_map<int, std::shared_ptr<ClientHandler>> clients;
    // std::unordered_map<int, std::shared_ptr<gameSession>> races;
    std::unordered_map<int, std::shared_ptr<Race>> races;
    std::mutex mtx;
    int race_id = 0;

public:
    void add_client(int id, std::shared_ptr<ClientHandler> client) {
        std::lock_guard<std::mutex> lock(mtx);
        clients[id] = client;
    }

    void remove(int id) {
        std::lock_guard<std::mutex> lock(mtx);
        clients.erase(id);
    }

    void remove_if_dead() {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto it = clients.begin(); it != clients.end();) {
            if (it->second->is_dead()) {
                it = clients.erase(it);
            } else {
                ++it;
            }
        }
    }

    std::shared_ptr<ClientHandler> get_client(int id) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = clients.find(id);
        return (it == clients.end()) ? nullptr : it->second;
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mtx);
        clients.clear();
        races.clear();
    }

    // Lo comento para que compile y hacer la primer entrega, despues vemos si
    // queda este o el otro

    void broadcast_race_state(const ServerMessageDTO& msg, int race_id) {

        std::shared_ptr<Race> race;
        // Tomamos el lock global solo para obtener un puntero seguro a la carrera
        {
            std::lock_guard<std::mutex> lock(mtx);
            auto it = races.find(race_id);
            if (it == races.end())
                return;
            race = it->second;
        }

        // bloqueo solo el mutex de la carrera para hacer broadcast
        std::lock_guard<std::mutex> lock_race(race->mtx);
        for (auto& client: race->clients) {
            if (!client->is_dead()) {
                client->send_msg(msg);
            }
        }
    }


    int create_race() {
        std::lock_guard<std::mutex> lock(mtx);
        int assigned_id = race_id;
        races[assigned_id] = std::make_shared<Race>();
        race_id++;
        return assigned_id;
    }

    bool add_client_to_race(int client_id, int race_id) {
        std::shared_ptr<Race> race;
        std::shared_ptr<ClientHandler> client;

        std::lock_guard<std::mutex> lock(mtx);
        auto client_it = clients.find(client_id);
        if (client_it == clients.end()) {
            return false;
        }
        auto race_it = races.find(race_id);
        if (race_it == races.end()) {
            return false;
        }
        race = race_it->second;
        client = client_it->second;

        std::lock_guard<std::mutex> lock_race(race->mtx);
        if (race->clients.size() >= MAX_PLAYERS_RACE) {
            return false;
        }
        race->clients.push_back(client);
        client->set_race_id(race_id);
        return true;
    }

    void remove_client_from_race(int client_id) {
        std::lock_guard<std::mutex> lock_global(mtx);

        auto client_it = clients.find(client_id);
        if (client_it == clients.end())
            return;
        int race_id_client = client_it->second->get_race_id();
        auto race_it = races.find(race_id_client);
        if (race_it == races.end())
            return;

        std::shared_ptr<Race> race = race_it->second;
        std::shared_ptr<ClientHandler> client = client_it->second;

        std::lock_guard<std::mutex> lock_race(race->mtx);

        auto& race_clients = race->clients;
        auto client_in_race = std::find(race_clients.begin(), race_clients.end(), client);

        if (client_in_race != race_clients.end()) {
            race_clients.erase(client_in_race);
        }
        client->set_race_id(-1);
    }

    void remove_race(int race_id) {
        std::lock_guard<std::mutex> lock_global(mtx);

        auto race_it = races.find(race_id);
        if (race_it == races.end())
            return;

        std::shared_ptr<Race> race = race_it->second;

        std::lock_guard<std::mutex> lock_race(race->mtx);
        race->clients.clear();

        races.erase(race_it);
    }

    std::vector<int> get_players_id(int race_id) {
        std::vector<int> players_id;
        std::shared_ptr<Race> race;

        {
            std::lock_guard<std::mutex> lock(mtx);
            auto it = races.find(race_id);
            if (it == races.end())
                return {};
            race = it->second;
        }

        {
            std::lock_guard<std::mutex> lock(race->mtx);
            std::transform(
                    race->clients.begin(), race->clients.end(), std::back_inserter(players_id),
                    [](const std::shared_ptr<ClientHandler>& client) { return client->get_id(); });
        }

        return players_id;
    }

    void set_game_queue(int race_id,
                        std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> new_queue) {
        std::shared_ptr<Race> race;

        {
            std::lock_guard<std::mutex> lock(mtx);
            auto it = races.find(race_id);
            if (it == races.end())
                return;
            race = it->second;
        }

        {
            std::lock_guard<std::mutex> lock(race->mtx);
            for (const auto& client: race->clients) {
                client->set_game_queue(new_queue);
            }
        }
    }
};
#endif
