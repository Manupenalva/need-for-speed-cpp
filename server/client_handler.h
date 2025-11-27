#ifndef CLIENT_HANDLER
#define CLIENT_HANDLER

#include <memory>
#include <string>
#include <vector>

#include "../common/protocol.h"
#include "../common/queue.h"
#include "../common/socket.h"
#include "../common/thread.h"
#include "events/clienthandlermessage.h"

#include "receiver.h"
#include "sender.h"

class ClientHandler {

public:
    ClientHandler(const int id, Socket s,
                  std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> queue);
    ~ClientHandler();

    void send_msg(ServerMessageDTO msg);

    void set_game_queue(std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> new_queue);

    bool is_dead();

    int get_id() const { return id; }

    void set_race_id(int race_id);
    void set_username(const std::string& username);
    std::string const get_username();
    int get_race_id() const;

private:
    const int id;
    Protocol protocol;
    Sender sender;
    Receiver receiver;
    int race_id;
    std::string username;
};
#endif
