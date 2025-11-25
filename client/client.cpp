#include "client.h"

#include <iostream>

Client::Client(Protocol& protocol, const int id):
        keep_running(true),
        protocol(protocol),
        events_queue(),
        server_queue(),
        sender(protocol, events_queue),
        receiver(protocol, server_queue),
        window(TITTLE_CLIENT),
        texture_manager(window.get_renderer()),
        sounds_manager(),
        sounds_events_handler(sounds_manager, id),
        drawer(window.get_renderer(), texture_manager, id),
        kb_reader(events_queue),
        actual_state() {
    init_game_handlers();
}


void Client::run() {
    init_resources();
    sender.start();
    receiver.start();
    GameLoopTimer timer(TARGET_FPS);
    try {
        // Variables de iteración
        uint32_t iterations_ahead = 0;
        sounds_manager.play_music(MAIN_MUSIC);

        while (keep_running) {
            if (!receiver.is_alive() || !sender.is_alive()) {
                break;  // En caso de que alguno de los hilos falle, salir del loop
            }

            // Procesar todos los mensajes entrantes y actualizar el estado interno
            kb_reader.listen_to_keyboard(keep_running, actual_state.upgrades_interval,
                                         actual_state.is_in_race);

            // Ejecutar una iteración lógica (actualizaciones locales / físicas)
            update_state_from_server();

            // Dibujar el frame actual (si hay estado)
            update_animation_frames(iterations_ahead);

            // Decidir cuántas iteraciones lógicas hay que adelantar según retraso
            timer.sleep_and_calc_next_it(iterations_ahead);
        }
        stop();
    } catch (const std::exception& e) {
        stop();
    }
}

void Client::stop() {
    protocol.shutdown_receive();
    events_queue.close();
    server_queue.close();
    sender.stop();
    receiver.stop();
    sender.join();
    receiver.join();
    keep_running = false;
}

void Client::init_resources() {
    texture_manager.load_resources();
    sounds_manager.load_music();
    sounds_manager.load_effects();
}

void Client::clear_display() { window.clear(); }

void Client::update_state_from_server() {
    // Carga las actualizaciones del estado enviadas por el servidor, siempre se dibuja la ultima
    // enviada.
    ServerMessageDTO server_msg;
    while (server_queue.try_pop(server_msg)) {
        auto handler_it = msg_handlers.find(server_msg.type);
        if (handler_it != msg_handlers.end()) {
            handler_it->second(server_msg);
        }
    }
}

void Client::update_animation_frames(int iterations_ahead) {

    if (actual_state.has_last_state && actual_state.is_in_race) {
        // Dibuja el ultimo estado recibido del servidor mientras esta en carrera.
        clear_display();
        drawer.update_game_state(actual_state.message, iterations_ahead, map_id);
        window.present();

        sounds_events_handler.process_message(actual_state.message);
        actual_state.has_last_state = false;
    }
    if (!actual_state.is_in_race && actual_state.has_last_state) {
        // Dibujar estadisticas
        clear_display();
        drawer.update_statistics_screen(actual_state.message);
        window.present();
        actual_state.has_last_state = false;
    }
    if (!actual_state.is_in_race && actual_state.upgrades_interval) {
        // Mostrar pantalla de mejora de auto
        clear_display();
        drawer.show_upgrade_screen(actual_state.message);
        window.present();
    }
}

void Client::init_game_handlers() {
    auto handler_state = [this](const ServerMessageDTO& server_msg) {
        actual_state.message = server_msg;
        actual_state.has_last_state = true;
    };
    msg_handlers[MsgType::STATE_UPDATE] = handler_state;

    msg_handlers[MsgType::RACE_POSITIONS] = handler_state;

    msg_handlers[MsgType::ACCUMULATED_POSITIONS] = handler_state;

    msg_handlers[MsgType::RACE_STARTED] = [this](const ServerMessageDTO& server_msg) {
        actual_state.is_in_race = true;
        actual_state.message = server_msg;
    };

    msg_handlers[MsgType::RACE_FINISHED] = [this](const ServerMessageDTO& server_msg) {
        actual_state.is_in_race = false;
        sounds_events_handler.final_game_sound();
        actual_state.message = server_msg;
    };

    msg_handlers[MsgType::SEND_MAP_NUMBER] = [this](const ServerMessageDTO& server_msg) {
        map_id = static_cast<MapType>(server_msg.map_number);
    };

    msg_handlers[MsgType::SEND_MINIMAP_INFO] = [this](const ServerMessageDTO& server_msg) {
        texture_manager.load_minimap_info(server_msg.minimap_info, map_id);
    };

    msg_handlers[MsgType::INTERVAL_UPDATE] = [this](const ServerMessageDTO& server_msg) {
        actual_state.upgrades_interval = true;
        actual_state.message = server_msg;
    };

    msg_handlers[MsgType::INTERVAL_CLOSED] = [this](const ServerMessageDTO& server_msg) {
        actual_state.upgrades_interval = false;
        actual_state.message = server_msg;
    };

    msg_handlers[MsgType::GAME_END] = [this](const ServerMessageDTO& server_msg) {
        actual_state.message = server_msg;
        keep_running = false;
    };
}
