#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <map>
#include <string>

#define HALF(x) ((x) / 2)
#define FULL_ROTATION_DEGREES 360.0f

#define TARGET_FPS 60

#define TITTLE_CLIENT "Need For Speed Race"
#define CONFIG_PATH "../client/config/game_config.yaml"

// Paths de imagenes
#define CAR_PATH "../client/resources/assets/cars/Cars.png"

#define LIBERTY_CITY_PATH "../client/resources/assets/cities/Backgrounds - Liberty City.png"
#define SAN_ANDREAS_PATH "../client/resources/assets/cities/Backgrounds - San Andreas.png"
#define VICE_CITY_PATH "../client/resources/assets/cities/Backgrounds - Vice City.png"

#define ARROW_PATH "../client/resources/assets/race/Flecha.png"
#define CHECKPOINT_PATH "../client/resources/assets/race/Checkpoint.png"
#define START_LINE_PATH "../client/resources/assets/race/Salida.png"

#define UPGRADE_SCREEN_PATH "../client/resources/assets/upgrade/Upgrade_Screen.png"

#define PATH_BURST "../client/resources/assets/animations/explosion.png"
#define PATH_FIRE "../client/resources/assets/animations/fuego.png"

enum MusicID { MAIN_MUSIC = 0 };

#define MUSIC_PATH "../client/resources/sounds/Need For Speed Music.mp3"

enum EffectID { CAR_CRASH = 0, CAR_BRAKE = 1, CAR_BURST = 2 };

const std::map<EffectID, std::string> EFFECTS_PATH = {
        {CAR_CRASH, "../client/resources/sounds/Car Crash Sound.wav"},
        {CAR_BRAKE, "../client/resources/sounds/Car Braking Sound.wav"},
        {CAR_BURST, "../client/resources/sounds/Car Burst Sound.wav"}};

// Map constants
#define MAP_MIN_X 0
#define MAP_MIN_Y 0

#endif  // CONSTANTS_H_
