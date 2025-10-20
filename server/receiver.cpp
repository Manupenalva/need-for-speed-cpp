#include "receiver.h"

#include "events/actionmessage.h"
#include "events/clienthandlermessage.h"
#include "events/createlobbymessage.h"
#include "events/getlobbiesmessage.h"
#include "events/joinlobbymessage.h"
#include "events/quitgamemessage.h"

Receiver::Receiver(Protocol& protocol,
                   std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> queue, int id):
        protocol(protocol), queue(queue), lobby_queue(queue), id(id) {
    this->start();
}

void Receiver::run() {
    while (should_keep_running() && protocol.socket_alive()) {
        try {
            ClientMessageDTO msg = protocol.recv_client_message();
            if (msg.type == MsgType::END_CONNECTION) {
                return;
            }
            std::shared_ptr<ClientHandlerMessage> game_message;
            switch (msg.type) {
                case MsgType::DRIVING_EVENT:
                    game_message = std::make_shared<ActionMessage>(id, msg.events);
                    break;
                case MsgType::JOIN_RACE:
                    game_message = std::make_shared<JoinLobbyMessage>(id, msg.lobby_name);
                    break;
                case MsgType::EXIT_RACE:
                    game_message = std::make_shared<QuitGameMessage>(id);
                    break;
                case MsgType::CREATE_RACE:
                    game_message = std::make_shared<CreateLobbyMessage>(id, msg.lobby_name);
                    break;
                case MsgType::GET_LOBBIES:
                    game_message = std::make_shared<GetLobbiesMessage>(id);
                    break;
                default:
                    continue;
            }

            queue->push(game_message);
        } catch (const ClosedQueue& e) {
            queue = lobby_queue;
            continue;
        } catch (const std::exception& e) {
            stop();
            return;
        }
    }
}

void Receiver::set_queue(std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> new_queue) {
    queue = new_queue;
}

void Receiver::kill() {
    stop();
    try {
        protocol.shutdown_receive();
    } catch (const std::exception&
                     e) {  // En caso de que el socket ya este cerrado y no permita shutdown
        return;
    }
}

Receiver::~Receiver() {
    kill();
    join();
}
