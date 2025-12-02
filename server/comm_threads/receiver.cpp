#include "receiver.h"

#include "../events/actionmessage.h"
#include "../events/cheatmessage.h"
#include "../events/clienthandlermessage.h"
#include "../events/createlobbymessage.h"
#include "../events/getcatalogmessage.h"
#include "../events/getlobbyupdatemessage.h"
#include "../events/joinlobbymessage.h"
#include "../events/quitgamemessage.h"
#include "../events/selectcarmessage.h"
#include "../events/sendnamemessage.h"
#include "../events/setreadymessage.h"
#include "../events/startracemessage.h"

Receiver::Receiver(Protocol& protocol,
                   std::shared_ptr<Queue<std::shared_ptr<ClientHandlerMessage>>> queue, int id):
        protocol(protocol), queue(queue), lobby_queue(queue), id(id) {
    this->start();
}

void Receiver::run() {
    while (should_keep_running()) {
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
                    game_message = std::make_shared<JoinLobbyMessage>(id, msg.lobby_id);
                    break;
                case MsgType::EXIT_RACE:
                    game_message = std::make_shared<QuitGameMessage>(id);
                    break;
                case MsgType::CREATE_RACE:
                    game_message = std::make_shared<CreateLobbyMessage>(id);
                    break;
                case MsgType::START_RACE:
                    game_message = std::make_shared<StartRaceMessage>(id);
                    break;
                case MsgType::GET_LOBBY_UPDATE:
                    game_message = std::make_shared<GetLobbyUpdateMessage>(id);
                    break;
                case MsgType::SELECT_CAR:
                    game_message = std::make_shared<SelectCarMessage>(id, msg.car_id);
                    break;
                case MsgType::SET_READY:
                    game_message = std::make_shared<SetReadyMessage>(id);
                    break;
                case MsgType::GET_CAR_CATALOG:
                    game_message = std::make_shared<GetCatalogMessage>(id);
                    break;
                case MsgType::CHEAT_CODE:
                    game_message = std::make_shared<CheatMessage>(id, msg.cheat_code);
                    break;
                case MsgType::SEND_NAME:
                    game_message = std::make_shared<SendNameMessage>(id, msg.name);
                    break;
                default:
                    continue;
            }

            queue->push(game_message);
        } catch (const ClosedQueue& e) {
            if (queue == lobby_queue) {
                return;
            }
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
    if (should_keep_running()) {
        stop();
    }
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
