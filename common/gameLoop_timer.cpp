#include "gameLoop_timer.h"

GameLoopTimer::GameLoopTimer(uint32_t target_fps):
        target_frame_duration_ms(MS_PER_SECOND / target_fps), last_frame_time_ms(SDL_GetTicks()) {}

void GameLoopTimer::sleep_and_calc_next_it(uint32_t& it) {
    uint32_t current_time = SDL_GetTicks();
    uint32_t frame_time = current_time - last_frame_time_ms;

    if (frame_time < target_frame_duration_ms) {
        SDL_Delay(target_frame_duration_ms - frame_time);
        last_frame_time_ms = current_time + (target_frame_duration_ms - frame_time);
        it = 1;
    } else {
        last_frame_time_ms = current_time;
        it = frame_time / target_frame_duration_ms + 1;
    }
}
