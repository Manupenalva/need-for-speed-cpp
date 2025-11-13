#ifndef SOUNDS_EVENTSHANDLER_H
#define SOUNDS_EVENTSHANDLER_H

#include <chrono>
#include <cmath>
#include <map>
#include <unordered_map>

#include "../../common/messageDTOs.h"

#include "sounds_manager.h"

#define MAX_RANGE 500.0f
#define MIN_VOLUME 0.0f
#define MAX_VOLUME 1.0f
#define WITHOUT_LOOPS 0
#define SOUND_COOLDOWN_MS 200

class SoundsEventsHandler {
private:
    SoundsManager& soundsManager;
    int client_car_id;
    std::map<int, bool> prev_crash_states;  // Mapa para rastrear estados previos de choque
    std::map<int, bool> prev_brake_states;  // Mapa para rastrear estados previos de frenado
    std::map<int, bool> prev_burst_states;  // Mapa para rastrear estados previos de explosión
    std::unordered_map<EffectID, std::chrono::steady_clock::time_point> last_played_times;

    void process_car_state(const CarState& car, float client_x, float client_y);

    float compute_volume(float x1, float y1, float x2, float y2);

    bool can_play_sound(EffectID id);

public:
    SoundsEventsHandler(SoundsManager& manager, int client_id);

    void process_message(const ServerMessageDTO& msg);
};

#endif
