#include "protocol.h"

#include <algorithm>
#include <vector>

Protocol::Protocol(Socket& socket):
        socket(socket), sender(MessageSender(socket)), receiver(MessageReceiver(socket)) {}

void Protocol::send_client_message(const ClientMessageDTO& msg) { sender.send_message(msg); }

void Protocol::send_server_message(const ServerMessageDTO& msg) { sender.send_message(msg); }

ClientMessageDTO Protocol::recv_client_message() { return receiver.recv_client_message(); }

ServerMessageDTO Protocol::recv_server_message() { return receiver.recv_server_message(); }
