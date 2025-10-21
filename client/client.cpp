#include "client.h"

Client::Client(const char* hostname, const char* servname, const int id):
        id(id),
        protocol(servname, hostname),
        events_queue(),
        server_queue(),
        sender(protocol, events_queue),
        receiver(protocol, server_queue),
        window(TITTLE_CLIENT, 800, 600),
        texture_manager(window.get_renderer(), PATH),
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
            kb_reader.listen_to_keyboard(_keep_running);
            // Procesar todos los mensajes entrantes y actualizar el estado interno
            // Ejecutar una iteración lógica (actualizaciones locales / físicas)
            update_state_from_server();

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

void Client::init_resources() {
    texture_manager.load_resources();

    // Dibuja un estado inicial con un auto en el centro
    ServerMessageDTO msg;
    msg.type = MsgType::STATE_UPDATE;
    CarState car;
    car.id = 0;
    car.x = 400.0f;
    car.y = 300.0f;
    car.angle = 270.0f;  // orientación
    car.speed = 0.0f;
    car.lap = 0;
    msg.state.cars.push_back(car);

    // Actualizar drawer con el estado y dibujar
    drawer.update_state(msg, 0);
    window.present();
}

void Client::clear_display() { window.clear(); }

void Client::update_state_from_server() {
    // Carga las actualizaciones del estado enviadas por el servidor, siempre se dibuja la ultima
    // enviada.
    ServerMessageDTO server_msg;
    while (server_queue.try_pop(server_msg)) {
        if (server_msg.type != MsgType::STATE_UPDATE) {
            continue;
        }
        last_state = server_msg;
        has_last_state = true;
    }
}

void Client::update_animation_frames(int iterations_ahead) {
    // Si tenemos un estado para dibujar, pedir al drawer que lo pinte
    if (has_last_state) {
        // Limpiar la pantalla antes de dibujar (solo cuando hay un nuevo estado)
        clear_display();
        drawer.update_state(last_state, iterations_ahead);
        window.present();
        has_last_state = false;  // Ya se dibujó este estado
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
