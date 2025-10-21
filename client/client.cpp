#include "client.h"

Client::Client(const char* hostname, const char* servname, const int id):
        id(id),
        protocol(hostname, servname),
        events_queue(),
        server_queue(),
        sender(protocol, events_queue),
        receiver(protocol, server_queue),
        texture_manager(window.get_renderer(), "assets/"),
        window(TITTLE_CLIENT, 800, 600),
        drawer(window.get_renderer(), texture_manager, id),
        kb_reader(events_queue),
        last_state(),
        has_last_state(false) {}


void Client::run() {
    init_resources();
    sender.start();
    receiver.start();
    try {
        // Variables de iteración
        int iterations_ahead = 0;

        while (_keep_running) {
            read_from_keyboard();
            // Procesar todos los mensajes entrantes y actualizar el estado interno
            // Ejecutar una iteración lógica (actualizaciones locales / físicas)
            update_state_from_server();

            // Limpiar la pantalla antes de dibujar
            clear_display();

            // Dibujar el frame actual (si hay estado)
            uint32_t render_start = SDL_GetTicks();
            update_animation_frames(iterations_ahead);

            uint32_t render_end = SDL_GetTicks();
            uint32_t render_time = render_end - render_start;

            // Decidir cuántas iteraciones lógicas hay que adelantar según retraso
            iterations_ahead = sleep_and_calc_next_it(render_time);
        }
        stop();
    } catch (const std::exception& e) {
        stop();
    }
}

void Client::stop() {
    sender.stop();
    receiver.stop();
    sender.join();
    receiver.join();
    protocol.shutdown_receive();
    _keep_running = false;
    _is_alive = false;
}

void Client::init_resources() { texture_manager.load_resources(); }

void Client::read_from_keyboard() {
    // el kb_reader se encarga de leer los eventos y mandarlos al sender
    kb_reader.listen_to_keyboard();
}

void Client::clear_display() { window.clear(); }

void Client::update_state_from_server() {
    // Carga las actualizaciones del estado enviadas por el servidor, siempre se dibuja la ultima
    // enviada.
    ServerMessageDTO server_msg;
    while (server_queue.try_pop(server_msg)) {
        last_state = server_msg;
        has_last_state = true;
    }
}

void Client::update_animation_frames(int iterations_ahead) {
    // Si tenemos un estado para dibujar, pedir al drawer que lo pinte
    if (has_last_state) {
        drawer.update_state(last_state, iterations_ahead);
        window.present();
    } else {
        // Si no hay estado, presentar el buffer actual
        window.present();
    }
}

int Client::sleep_and_calc_next_it(uint32_t render_time_ms) {
    // Objetivo: 60 FPS -> frame_ms por iteración
    const uint32_t frame_ms = FRAME_MS;
    int ahead = 1;

    if (render_time_ms < frame_ms) {
        // No esta atradaso, puede dormir hasta completar el frame.
        std::this_thread::sleep_for(std::chrono::milliseconds(frame_ms - render_time_ms));
    } else {
        // Está atrasado, calcular cuántas iteraciones lógicas hay que
        // adelantar para ponerse al día.
        ahead = render_time_ms / frame_ms;

        // duerme hasta completar la iteracion actual
        uint32_t extra_time = render_time_ms % frame_ms;
        if (extra_time > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(frame_ms - extra_time));
        }
    }

    return ahead;
}
