#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <vector>


class Gameloop: public Thread {
private:
    Queue<ActionMessage>& user_commands_queue;
    std::list<ClientHandler*>& players;
    std::unordered_map<int, CarState> players_state;
    uint32_t frame;


public:
    Gameloop(Queue<ClientHandlerMessage>& user_commands_queue, std::list<ClientHandler*>& players);
    void run() override;

private:
    void broadcast_players();
    void update_positions();
    void update_car_stats(const int& player_id, const uint8_t& action);

    Gameloop(const Gameloop& other) = delete;
    Gameloop& operator=(const Gameloop& other) = delete;
};

#endif
