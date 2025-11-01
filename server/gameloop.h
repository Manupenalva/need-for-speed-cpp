#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include <box2d/box2d.h>

#include "../common/carState.h"
#include "../common/queue.h"
#include "../common/thread.h"
#include "events/clienthandlermessage.h"

#include "car.h"
#include "monitorclients.h"


class Gameloop: public Thread {
private:
    std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> user_commands_queue;
    MonitorClients& games_monitor;
    int game_id;
    std::unordered_map<uint16_t, std::unique_ptr<Car>> players_cars;
    uint32_t frames;
    b2WorldId world;


public:
    Gameloop(std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> user_commands_queue,
             MonitorClients& games_monitor, int game_id);
    void run() override;

private:
    void broadcast_players();
    void update_positions();
    void update_car_input(const uint16_t& player_id, const uint8_t& action);

    Gameloop(const Gameloop& other) = delete;
    Gameloop& operator=(const Gameloop& other) = delete;
};

#endif
