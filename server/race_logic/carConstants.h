#ifndef CAR_CONSTANTS_H
#define CAR_CONSTANTS_H

struct CarConstants {
    struct Car {
        float MAX_SPEED;
        float MAX_ACCELERATION;
        float MAX_HEALTH;
        float MAX_MASS;
        float MAX_DRIVABILITY;
    } car;

    struct Physics {
        float BASE_MAX_SPEED;
        float BASE_ACCELERATION;
        float BASE_ANGLE_ROTATION;
        float BASE_FRICTION;
        float MIN_SPEED;
        float LIGHT_CRASH_DAMAGE;
        float MEDIUM_CRASH_DAMAGE;
        float HEAVY_CRASH_DAMAGE;
        float MEDIUM_CRASH_LIMIT;
        float HEAVY_CRASH_LIMIT;
        float REVERSE_ACCELERATION_FACTOR;
        float REVERSE_SPEED_FACTOR;
        float MIN_GAME_SPEED;
        float NITRO_USE;
        float NITRO_ACCELERATION_FACTOR;
    } physics;
};
#endif
