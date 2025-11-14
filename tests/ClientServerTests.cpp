#include <gtest/gtest.h>

#include "../common/messagereceiver.h"
#include "../common/messagesender.h"

#include "MemorySocket.h"

class ProtocolTestClient: public ::testing::Test {
protected:
    MemorySocket mem_socket;
    MessageSender sender;
    MessageReceiver receiver;

    ProtocolTestClient(): sender(mem_socket), receiver(mem_socket) {}

    void TearDown() override {
        EXPECT_TRUE(mem_socket.is_empty()) << "MemorySocket buffer is not empty after test.";
    }
};

TEST_F(ProtocolTestClient, JoinRace) {

    ClientMessageDTO send_msg;
    send_msg.type = MsgType::JOIN_RACE;
    send_msg.lobby_id = 42;

    sender.send_message(send_msg);

    ClientMessageDTO recv_msg = receiver.recv_client_message();

    EXPECT_EQ(recv_msg.type, MsgType::JOIN_RACE);
    EXPECT_EQ(recv_msg.lobby_id, 42);
}

TEST_F(ProtocolTestClient, SelectCar) {

    ClientMessageDTO send_msg;
    send_msg.type = MsgType::SELECT_CAR;
    send_msg.car_id = 7;

    sender.send_message(send_msg);

    ClientMessageDTO recv_msg = receiver.recv_client_message();

    EXPECT_EQ(recv_msg.type, MsgType::SELECT_CAR);
    EXPECT_EQ(recv_msg.car_id, 7);
}

TEST_F(ProtocolTestClient, DrivingEvent) {

    ClientMessageDTO send_msg;
    send_msg.type = MsgType::DRIVING_EVENT;
    send_msg.events = {1, 2, 3};

    sender.send_message(send_msg);

    ClientMessageDTO recv_msg = receiver.recv_client_message();

    EXPECT_EQ(recv_msg.type, MsgType::DRIVING_EVENT);
    EXPECT_EQ(recv_msg.events, std::vector<uint8_t>({1, 2, 3}));
}

TEST_F(ProtocolTestClient, CodeMessages) {
    std::vector<MsgType> msg_types = {MsgType::CREATE_RACE,      MsgType::EXIT_RACE,
                                      MsgType::GET_LOBBIES,      MsgType::START_RACE,
                                      MsgType::GET_LOBBY_UPDATE, MsgType::GET_CAR_CATALOG,
                                      MsgType::SET_READY};
    for (const auto& msg_type: msg_types) {
        ClientMessageDTO send_msg;
        send_msg.type = msg_type;
        sender.send_message(send_msg);

        ClientMessageDTO recv_msg = receiver.recv_client_message();
        EXPECT_EQ(recv_msg.type, msg_type);
    }
}
