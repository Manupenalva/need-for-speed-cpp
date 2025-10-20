#ifndef CLIENT_HANDLER
#define CLIENT_HANDLER

#include <memory>
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

    void send_msg(ServerMessageDTO msg);

    bool is_dead();

    int get_id() const { return id; }

private:
    const int id;
    Protocol protocol;
    Sender sender;
    Receiver receiver;
};
#endif
