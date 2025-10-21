#include "protocol.h"

#include <algorithm>
#include <utility>
#include <vector>

Protocol::Protocol(Socket socket):
        socket(std::move(socket)),
        sender(MessageSender(this->socket)),
        receiver(MessageReceiver(this->socket)) {}

Protocol::Protocol(const char* port, const char* ip_address):
        socket(Socket(ip_address, port)),
        sender(MessageSender(this->socket)),
        receiver(MessageReceiver(this->socket)) {}

void Protocol::send_client_message(const ClientMessageDTO& msg) { sender.send_message(msg); }

void Protocol::send_server_message(const ServerMessageDTO& msg) { sender.send_message(msg); }

ClientMessageDTO Protocol::recv_client_message() { return receiver.recv_client_message(); }

ServerMessageDTO Protocol::recv_server_message() { return receiver.recv_server_message(); }

bool Protocol::socket_alive() {
    return (!socket.is_stream_send_closed() && !socket.is_stream_recv_closed());
}

void Protocol::shutdown_receive() { socket.shutdown(SHUT_RD); }
