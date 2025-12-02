
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../common/messageDTOs.h"
#include "../common/messagereceiver.h"
#include "../common/messagesender.h"

#include "MemorySocket.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::SetArrayArgument;

class ProtocolTestServer: public ::testing::Test {
protected:
    MemorySocket mem_socket;
    MessageSender sender;
    MessageReceiver receiver;

    ProtocolTestServer(): sender(mem_socket), receiver(mem_socket) {}

    void TearDown() override {
        EXPECT_TRUE(mem_socket.is_empty()) << "MemorySocket buffer is not empty after test. "
                                           << mem_socket.buffer.size() << " bytes remaining.";
    }
};

bool float_equal(float a, float b, float eps = 0.01f) { return std::fabs(a - b) < eps; }

TEST_F(ProtocolTestServer, JoinResult) {

    ServerMessageDTO send_msg;
    send_msg.type = MsgType::JOIN_RESULT;
    send_msg.result = true;

    sender.send_message(send_msg);

    ServerMessageDTO recv_msg = receiver.recv_server_message();

    EXPECT_EQ(recv_msg.type, MsgType::JOIN_RESULT);
    EXPECT_TRUE(recv_msg.result);
}

TEST_F(ProtocolTestServer, ClientID) {


    ServerMessageDTO send_msg;
    send_msg.type = MsgType::SEND_CLIENT_ID;
    send_msg.id = 15;

    sender.send_message(send_msg);

    ServerMessageDTO recv_msg = receiver.recv_server_message();

    EXPECT_EQ(recv_msg.type, MsgType::SEND_CLIENT_ID);
    EXPECT_EQ(recv_msg.id, 15);
}

bool verify_car_properties(const CarProperties& car1, const CarProperties& car2) {
    return (car1.car_id == car2.car_id) && (car1.max_speed == car2.max_speed) &&
           (car1.acceleration == car2.acceleration) && (car1.max_health == car2.max_health) &&
           (car1.mass == car2.mass) && (car1.control == car2.control);
}

TEST_F(ProtocolTestServer, CarCatalog) {

    ServerMessageDTO send_msg;
    send_msg.type = MsgType::SEND_CAR_CATALOG;
    CarProperties car1{1, 200, 10, 100, 1500, 80};
    CarProperties car2{2, 220, 12, 110, 1400, 85};
    send_msg.car_catalog = {car1, car2};

    sender.send_message(send_msg);

    ServerMessageDTO recv_msg = receiver.recv_server_message();

    EXPECT_EQ(recv_msg.type, MsgType::SEND_CAR_CATALOG);
    ASSERT_EQ(recv_msg.car_catalog.size(), 2);
    EXPECT_TRUE(verify_car_properties(recv_msg.car_catalog[0], car1));
    EXPECT_TRUE(verify_car_properties(recv_msg.car_catalog[1], car2));
}

TEST_F(ProtocolTestServer, MapNumber) {

    ServerMessageDTO send_msg;
    send_msg.type = MsgType::SEND_MAP_NUMBER;
    send_msg.map_number = 3;

    sender.send_message(send_msg);

    ServerMessageDTO recv_msg = receiver.recv_server_message();

    EXPECT_EQ(recv_msg.type, MsgType::SEND_MAP_NUMBER);
    EXPECT_EQ(recv_msg.map_number, 3);
}

bool verify_car_props(const CarProperties& car1, const CarProperties& car2) {
    return (car1.car_id == car2.car_id) && (car1.max_speed == car2.max_speed) &&
           (car1.acceleration == car2.acceleration) && (car1.max_health == car2.max_health) &&
           (car1.mass == car2.mass) && (car1.control == car2.control);
}
bool verify_player_state(const PlayerState& ps1, const PlayerState& ps2) {
    return (ps1.player_id == ps2.player_id) && (ps1.ready == ps2.ready) &&
           (ps1.previous_position == ps2.previous_position) &&
           (ps1.result_time == ps2.result_time) &&
           (ps1.next_penalization_time == ps2.next_penalization_time) &&
           verify_car_props(ps1.car_properties, ps2.car_properties);
}

TEST_F(ProtocolTestServer, IntervalState) {

    ServerMessageDTO send_msg;
    send_msg.type = MsgType::INTERVAL_UPDATE;
    IntervalState interval_state;
    interval_state.players_ready = 2;
    interval_state.total_players = 10;
    PlayerState player1{1, true, 1, 60.0f, 0.0f, {1, 200, 10, 100, 1500, 80}};
    PlayerState player2{2, false, 2, 62.0f, 5.0f, {2, 220, 12, 110, 1400, 85}};
    PlayerState player3{3, true, 3, 58.0f, 0.0f, {1, 200, 10, 100, 1500, 80}};
    interval_state.player_states = {player1, player2, player3};
    send_msg.interval_state = interval_state;

    sender.send_message(send_msg);

    ServerMessageDTO recv_msg = receiver.recv_server_message();

    EXPECT_EQ(recv_msg.type, MsgType::INTERVAL_UPDATE);
    EXPECT_EQ(recv_msg.interval_state.players_ready, 2);
    EXPECT_EQ(recv_msg.interval_state.total_players, 10);
    ASSERT_EQ(recv_msg.interval_state.player_states.size(), 3);
    EXPECT_TRUE(verify_player_state(recv_msg.interval_state.player_states[0], player1));
    EXPECT_TRUE(verify_player_state(recv_msg.interval_state.player_states[1], player2));
    EXPECT_TRUE(verify_player_state(recv_msg.interval_state.player_states[2], player3));
}

bool verify_checkpoint_info(const CheckpointInfo& cp1, const CheckpointInfo& cp2) {
    return (cp1.id == cp2.id) && (cp1.x == cp2.x) && (cp1.y == cp2.y) && (cp1.angle == cp2.angle) &&
           (cp1.radius == cp2.radius) && (cp1.type == cp2.type);
}

bool verify_checkpoint_arrow(const CheckpointArrow& ca1, const CheckpointArrow& ca2) {
    return (ca1.x == ca2.x) && (ca1.y == ca2.y) && (ca1.angle == ca2.angle);
}

bool verify_car_state(const CarState& car1, const CarState& car2) {
    return (car1.id == car2.id) && (car1.x == car2.x) && (car1.y == car2.y) &&
           (car1.angle == car2.angle) && (car1.speed == car2.speed) && (car1.lap == car2.lap) &&
           verify_checkpoint_info(car1.checkpoint, car2.checkpoint) &&
           verify_checkpoint_arrow(car1.checkpoint_arrow, car2.checkpoint_arrow) &&
           (car1.crashed == car2.crashed) && (car1.under_bridge == car2.under_bridge) &&
           (car1.braking == car2.braking) && (car1.car_type == car2.car_type) &&
           (car1.health == car2.health) && (car1.exploded == car2.exploded) &&
           (car1.max_health == car2.max_health);
}

bool verify_npc_state(const NpcState& npc1, const NpcState& npc2) {
    return (npc1.x == npc2.x) && (npc1.y == npc2.y) && (npc1.angle == npc2.angle) &&
           (npc1.car_type == npc2.car_type) && (npc1.under_bridge == npc2.under_bridge);
}

TEST_F(ProtocolTestServer, StateUpdate) {

    ServerMessageDTO send_msg;
    send_msg.type = MsgType::STATE_UPDATE;
    State state;
    state.countdown_time = 5;
    state.frame = 100;
    state.num_cars = 2;
    CheckpointInfo cp1{1, 120.0f, 60.0f, 0.0f, 5.0f, 0};
    CheckpointArrow ca1{125.0f, 62.0f, 0.0f};
    CheckpointInfo cp2{2, 110.0f, 55.0f, 10.0f, 5.0f, 0};
    CheckpointArrow ca2{115.0f, 57.0f, 10.0f};
    CarState car1(1, 150.0f, 75.0f, 0.0f, 0.0f, 2, cp1, ca1, false, false, true, true, 1, 200, 200);
    CarState car2(2, 140.0f, 70.0f, 10.0f, 5.0f, 90.0f, cp2, ca2, false, true, true, true, 1, 200,
                  300);
    state.cars = {car1, car2};
    NpcState npc1{130.0f, 65.0f, 50.0f, 1, true};
    state.npcs = {npc1};
    state.remaining_time = 150.0f;
    send_msg.state = state;

    sender.send_message(send_msg);

    ServerMessageDTO recv_msg = receiver.recv_server_message();

    EXPECT_EQ(recv_msg.type, MsgType::STATE_UPDATE);
    EXPECT_EQ(recv_msg.state.countdown_time, 5);
    EXPECT_EQ(recv_msg.state.frame, 100);
    EXPECT_EQ(recv_msg.state.num_cars, 2);
    ASSERT_EQ(recv_msg.state.cars.size(), 2);
    EXPECT_TRUE(verify_car_state(recv_msg.state.cars[0], car1));
    EXPECT_TRUE(verify_car_state(recv_msg.state.cars[1], car2));
    ASSERT_EQ(recv_msg.state.npcs.size(), 1);
    EXPECT_TRUE(verify_npc_state(recv_msg.state.npcs[0], npc1));
    EXPECT_TRUE(float_equal(recv_msg.state.remaining_time, 150.0f));
}

TEST_F(ProtocolTestServer, CodeMessages) {
    std::vector<MsgType> msg_types = {MsgType::GAME_START, MsgType::GAME_END,
                                      MsgType::RACE_FINISHED, MsgType::RACE_STARTED,
                                      MsgType::INTERVAL_CLOSED};
    for (const auto& msg_type: msg_types) {
        ServerMessageDTO send_msg;
        send_msg.type = msg_type;
        sender.send_message(send_msg);

        ServerMessageDTO recv_msg = receiver.recv_server_message();
        EXPECT_EQ(recv_msg.type, msg_type);
    }
    for (const auto& msg_type: msg_types) {
        ClientMessageDTO send_msg;
        send_msg.type = msg_type;
        sender.send_message(send_msg);

        ClientMessageDTO recv_msg = receiver.recv_client_message();
        EXPECT_EQ(recv_msg.type, msg_type);
    }
}

bool verify_minimap_info(const MinimapInfo& mi1, const MinimapInfo& mi2) {

    for (size_t i = 0; i < mi1.checkpoints.size(); ++i) {
        if (!verify_checkpoint_info(mi1.checkpoints[i], mi2.checkpoints[i])) {
            return false;
        }
    }
    for (size_t i = 0; i < mi1.arrows.size(); ++i) {
        if (!verify_checkpoint_arrow(mi1.arrows[i], mi2.arrows[i])) {
            return false;
        }
    }
    return true;
}

TEST_F(ProtocolTestServer, MinimapInfo) {

    ServerMessageDTO send_msg;
    send_msg.type = MsgType::SEND_MINIMAP_INFO;
    MinimapInfo minimap_info;

    CheckpointInfo cp1{1, 50.0f, 50.0f, 0.0f, 10.0f, 0};
    CheckpointInfo cp2{2, 150.0f, 50.0f, 0.0f, 10.0f, 0};
    minimap_info.checkpoints = {cp1, cp2};
    CheckpointArrow ca1{60.0f, 60.0f, 0.0f};
    CheckpointArrow ca2{160.0f, 60.0f, 0.0f};
    minimap_info.arrows = {ca1, ca2};

    send_msg.minimap_info = minimap_info;

    sender.send_message(send_msg);

    ServerMessageDTO recv_msg = receiver.recv_server_message();

    EXPECT_EQ(recv_msg.type, MsgType::SEND_MINIMAP_INFO);
    ASSERT_EQ(recv_msg.minimap_info.checkpoints.size(), 2);
    ASSERT_EQ(recv_msg.minimap_info.arrows.size(), 2);
    EXPECT_TRUE(verify_minimap_info(recv_msg.minimap_info, minimap_info));
}

bool verify_positions(const std::vector<ResultInfo>& pos1, const std::vector<ResultInfo>& pos2) {
    if (pos1.size() != pos2.size()) {
        return false;
    }
    for (size_t i = 0; i < pos1.size(); ++i) {
        if (pos1[i].id != pos2[i].id || !float_equal(pos1[i].time, pos2[i].time) ||
            !float_equal(pos1[i].penalization_time, pos2[i].penalization_time) ||
            pos1[i].name != pos2[i].name) {
            return false;
        }
    }
    return true;
}

TEST_F(ProtocolTestServer, RacePositions) {
    ServerMessageDTO send_msg;
    send_msg.type = MsgType::RACE_POSITIONS;
    send_msg.positions = {{1, 120.5f, 45.0f, "juan"},
                          {2, 130.0f, 20.3f, "charlie"},
                          {3, 110.75f, 10.0f, "Peter"}};
    sender.send_message(send_msg);

    ServerMessageDTO recv_msg = receiver.recv_server_message();
    EXPECT_EQ(recv_msg.type, MsgType::RACE_POSITIONS);
    ASSERT_EQ(recv_msg.positions.size(), 3);
    EXPECT_TRUE(verify_positions(recv_msg.positions, send_msg.positions));
}

TEST_F(ProtocolTestServer, AccumulatedPositions) {

    ServerMessageDTO send_msg;
    send_msg.type = MsgType::ACCUMULATED_POSITIONS;
    send_msg.positions = {{1, 300.5f, 35.5f, "juan"},
                          {2, 320.0f, 32.3f, "lionel"},
                          {3, 280.75f, 45.0f, "andres"}};
    sender.send_message(send_msg);

    ServerMessageDTO recv_msg = receiver.recv_server_message();
    EXPECT_EQ(recv_msg.type, MsgType::ACCUMULATED_POSITIONS);
    ASSERT_EQ(recv_msg.positions.size(), 3);
    EXPECT_TRUE(verify_positions(recv_msg.positions, send_msg.positions));
}

TEST_F(ProtocolTestServer, NameResult) {

    ServerMessageDTO send_msg;
    send_msg.type = MsgType::NAME_RESULT;
    send_msg.result = true;

    sender.send_message(send_msg);

    ServerMessageDTO recv_msg = receiver.recv_server_message();

    EXPECT_EQ(recv_msg.type, MsgType::NAME_RESULT);
    EXPECT_TRUE(recv_msg.result);
}
