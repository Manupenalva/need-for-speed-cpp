#ifndef GAMESESSION_H
#define GAMESESSION_H


class GameSession{
private:
    std::list<std::unique_ptr<ClientHandler>> players;
    Queue<ActionMessage> user_commands_queue;
    Gameloop gameloop;

public:
    GameSession(std::list<std::unique_ptr<ClientHandler>>&& players);
    void start();
    void stop();

private:
    GameSession(const GameSession& other) = delete;
    GameSession& operator=(const GameSession& other) = delete;
};

#endif
