#include "client.h"

Client::Client(const char* hostname, const char* servname)
    : socket(hostname, servname),
      protocol(socket),
      events_queue(),
      server_queue(),
      sender(protocol, events_queue),
      receiver(protocol, server_queue),
      window("Client Window", 800, 600),
      drawer(window.get_renderer()) {}

void Client::run() {
    init_resources();  
    sender.start();
    receiver.start();
    try {
        while (_keep_running) {
            update_state_from_server();
            clear_display();
            update_animation_frames();
            render_frame();
            sleep_and_calc_next_it();
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
    socket.shutdown(SHUT_RDWR);
    socket.close();
    _keep_running = false;
    _is_alive = false;
}
