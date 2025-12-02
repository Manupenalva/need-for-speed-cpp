#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <memory>
#include <utility>

#include "monitors/monitorGames.h"
#include "race_logic/configConstants.h"
#include "race_logic/gameloop.h"

class GameSession {
private:
    std::shared_ptr<RaceStruct> race;
    int game_id;
    std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> user_commands_queue;
    Gameloop gameloop;

public:
    GameSession(const int& id, std::shared_ptr<RaceStruct> race,
                std::shared_ptr<ConfigConstants> config);
    void stop();
    ~GameSession();
    int get_id() const;

    bool is_running() const;

private:
    GameSession(const GameSession& other) = delete;
    GameSession& operator=(const GameSession& other) = delete;
};

#endif
