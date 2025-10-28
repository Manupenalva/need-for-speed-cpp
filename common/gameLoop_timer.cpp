#include "gameLoop_timer.h"

#include <chrono>
#include <thread>

GameLoopTimer::GameLoopTimer(uint32_t target_fps):
        target_frame_duration_ms(MS_PER_SECOND / target_fps), last_frame_time_ms(0) {
    last_frame_time_ms = get_current_time();
}

uint64_t GameLoopTimer::get_current_time() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now().time_since_epoch())
            .count();
}

void GameLoopTimer::sleep_and_calc_next_it(uint32_t& it) {
    uint64_t current_time = get_current_time();
    auto frame_time = current_time - last_frame_time_ms;

    if (frame_time < target_frame_duration_ms) {
        std::this_thread::sleep_for(
                std::chrono::milliseconds(target_frame_duration_ms - frame_time));
        last_frame_time_ms = current_time + (target_frame_duration_ms - frame_time);
        it = 1;
    } else {
        last_frame_time_ms = current_time;
        it = frame_time / target_frame_duration_ms + 1;
    }
}
