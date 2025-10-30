#ifndef ACCEPTOR
#define ACCEPTOR

#include <memory>
#include <string>

#include "../common/queue.h"
#include "../common/socket.h"
#include "../common/thread.h"
#include "events/clienthandlermessage.h"

#include "client_handler.h"
#include "monitorGames.h"
#include "monitorClients.h"
#include "receiver.h"

class Acceptor: public Thread {


private:
    Socket acceptor;
    std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> queue;
    int id;
    MonitorClients& clients;

    void reap();
    void clear();

public:
    Acceptor(const std::string& servname,
             std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> queue,
             MonitorClients& clients);
    Acceptor(const Acceptor&) = delete;
    Acceptor& operator=(const Acceptor&) = delete;
    ~Acceptor() override;
    void run() override;
    void shutdown();
};
#endif
