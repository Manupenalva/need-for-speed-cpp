#ifndef GAMESESSION_H
#define GAMESESSION_H


class GameSession{
private:
    MonitorGames& games_monitor;
    int game_id;
    std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> user_commands_queue;
    Gameloop gameloop;

public:
    GameSession(const MonitorGames& games_monitor, const int& id);
    void start();
    void stop();

private:
    GameSession(const GameSession& other) = delete;
    GameSession& operator=(const GameSession& other) = delete;
};

#endif
