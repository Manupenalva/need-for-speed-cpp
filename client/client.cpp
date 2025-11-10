#include "client.h"

Client::Client(Protocol& protocol, const int id):
        protocol(protocol),
        events_queue(),
        server_queue(),
        sender(protocol, events_queue),
        receiver(protocol, server_queue),
        window(TITTLE_CLIENT),
        texture_manager(window.get_renderer(), PATH),
        drawer(window.get_renderer(), texture_manager, id),
        kb_reader(events_queue),
        last_state(),
        has_last_state(false),
        is_in_race(true) {}


void Client::run() {
    init_resources();
    sender.start();
    receiver.start();
    GameLoopTimer timer(TARGET_FPS);
    try {
        // Variables de iteración
        uint32_t iterations_ahead = 0;

        while (_keep_running) {
            // Procesar todos los mensajes entrantes y actualizar el estado interno
            kb_reader.listen_to_keyboard(_keep_running, is_in_race);

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
    _keep_running = false;
    _is_alive = false;
}

void Client::init_resources() { texture_manager.load_resources(); }

void Client::clear_display() { window.clear(); }

void Client::update_state_from_server() {
    // Carga las actualizaciones del estado enviadas por el servidor, siempre se dibuja la ultima
    // enviada.
    ServerMessageDTO server_msg;
    while (server_queue.try_pop(server_msg)) {
        if (server_msg.type == MsgType::STATE_UPDATE) {
            last_state = server_msg;
            has_last_state = true;
        }
        if (server_msg.type == MsgType::RACE_STARTED) {
            is_in_race = true;
        }
        if (server_msg.type == MsgType::RACE_FINISHED) {
            is_in_race = false;
        }
    }
}

void Client::update_animation_frames(int iterations_ahead) {
    // Si tenemos un estado para dibujar y la carrera esta en curso, pedir al drawer que lo pinte
    if (has_last_state && is_in_race) {
        // Limpiar la pantalla antes de dibujar (solo cuando hay un nuevo estado)
        clear_display();
        drawer.update_game_state(last_state, iterations_ahead);
        window.present();
        has_last_state = false;  // Ya se dibujó este estado
    }
    if (!is_in_race) {
        // Mostrar pantalla de mejora de auto
        clear_display();
        drawer.show_upgrade_screen();
        window.present();
    }
}
