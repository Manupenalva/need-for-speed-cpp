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

#include "racemonitor.h"


class Gameloop: public Thread {
private:
    std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> user_commands_queue;
    std::shared_ptr<Race> race;
    std::unordered_map<uint16_t, CarInputState> players_cars;
    uint32_t frames;
    b2WorldId world;


public:
    Gameloop(std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> user_commands_queue,
             std::shared_ptr<Race> race);
    void run() override;

private:
    void broadcast_players();
    void update_car_physics(const uint16_t& player_id);
    void update_positions();
    void update_car_input(const uint16_t& player_id, const uint8_t& action);
    void broadcast_start();

    Gameloop(const Gameloop& other) = delete;
    Gameloop& operator=(const Gameloop& other) = delete;
};

#endif
