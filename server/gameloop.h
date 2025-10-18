#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <vector>
#include <cstdint>
#include "../common/carState.h"


class Gameloop: public Thread {
private:
    Queue<ActionMessage>& user_commands_queue;
    std::list<ClientHandler*>& players;
    std::unordered_map<uint16_t, CarInputState> players_cars;
    uint32_t frames;


public:
    Gameloop(Queue<ClientHandlerMessage>& user_commands_queue, std::list<ClientHandler*>& players);
    void run() override;

private:
    void broadcast_players();
    void update_car_state(const uint16_t& player_id);
    void update_positions();
    void update_car_input(const uint16_t& player_id, const uint8_t& action);

    Gameloop(const Gameloop& other) = delete;
    Gameloop& operator=(const Gameloop& other) = delete;
};

#endif
