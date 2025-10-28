#include <cstdint>

#include <SDL2/SDL.h>

#define MS_PER_SECOND 1000

class GameLoopTimer {
private:
    const uint32_t target_frame_duration_ms;
    uint64_t last_frame_time_ms;

public:
    explicit GameLoopTimer(uint32_t target_fps);

    void sleep_and_calc_next_it(uint32_t& it);

private:
    uint64_t get_current_time();
};
