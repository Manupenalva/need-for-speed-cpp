#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>
#include <memory>
#include <vector>

#include <netinet/in.h>

#include "SocketInterface.h"
#include "constants.h"
#include "messageDTOs.h"
#include "messagereceiver.h"
#include "messagesender.h"
#include "msgType.h"
#include "socket.h"
#include "state.h"

class Protocol {
public:
    explicit Protocol(Socket socket);
    Protocol(const char* port, const char* ip_address);
    ~Protocol() = default;

    Protocol(const Protocol&) = delete;
    Protocol& operator=(const Protocol&) = delete;

    void send_client_message(const ClientMessageDTO& msg);
    void send_server_message(const ServerMessageDTO& msg);

    ClientMessageDTO recv_client_message();
    ServerMessageDTO recv_server_message();

    bool socket_alive();
    void shutdown_receive();

private:
    std::unique_ptr<ISocket> socket;
    MessageSender sender;
    MessageReceiver receiver;
};

#endif
