#ifndef RACESTATUS_H
#define RACESTATUS_H

struct PlayerRaceStatus {
    bool has_finished;
    size_t current_checkpoint_index;
    size_t current_hint_index;
};

#endif
