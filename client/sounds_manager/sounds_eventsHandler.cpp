#include "sounds_eventsHandler.h"

SoundsEventsHandler::SoundsEventsHandler(SoundsManager& manager, int client_id):
        soundsManager(manager), client_car_id(client_id) {}


void SoundsEventsHandler::process_message(const ServerMessageDTO& msg) {
    if (msg.type != MsgType::STATE_UPDATE)
        return;

    auto client_car = std::find_if(msg.state.cars.begin(), msg.state.cars.end(),
                                   [this](const CarState& car) { return car.id == client_car_id; });

    if (client_car == msg.state.cars.end())
        return;

    float client_x = client_car->x;
    float client_y = client_car->y;

    for (const auto& car: msg.state.cars) {
        process_car_state(car, client_x, client_y);
    }
}

void SoundsEventsHandler::process_car_state(const CarState& car, float client_x, float client_y) {
    // Sonido de choque
    float volume = compute_volume(client_x, client_y, car.x, car.y);

    if (car.crashed && !prev_crash_states[car.id] && can_play_sound(CAR_CRASH)) {
        soundsManager.play_effect(CAR_CRASH, WITHOUT_LOOPS, volume);
    }
    prev_crash_states[car.id] = car.crashed;

    // Sonido de frenado (asumiendo que hay un atributo 'braking' en CarState)
    if (car.braking && !prev_brake_states[car.id] && can_play_sound(CAR_BRAKE)) {
        soundsManager.play_effect(CAR_BRAKE, WITHOUT_LOOPS, volume);
    }
    prev_brake_states[car.id] = car.braking;

    // Sonido de explosión
    if (car.exploded && !prev_burst_states[car.id] && can_play_sound(CAR_BURST)) {
        soundsManager.play_effect(CAR_BURST, WITHOUT_LOOPS, volume);
    }
    prev_burst_states[car.id] = car.exploded;
}

float SoundsEventsHandler::compute_volume(float x1, float y1, float x2, float y2) {
    // Calcular la distancia entre dos puntos
    float dx = x2 - x1;
    float dy = y2 - y1;
    float distance = std::sqrt(dx * dx + dy * dy);

    // Calcular el volumen basado en la distancia
    if (distance >= MAX_RANGE) {
        return MIN_VOLUME;
    }

    float volume = MAX_VOLUME - (distance / MAX_RANGE);
    return std::clamp(volume * volume, MIN_VOLUME, MAX_VOLUME);
}

bool SoundsEventsHandler::can_play_sound(EffectID id) {
    auto now = std::chrono::steady_clock::now();

    auto it = last_played_times.find(id);
    if (it == last_played_times.end()) {
        last_played_times[id] = now;
        return true;
    }

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->second);
    if (elapsed.count() >= SOUND_COOLDOWN_MS) {
        last_played_times[id] = now;
        return true;
    }

    return false;
}
