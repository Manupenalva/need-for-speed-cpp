#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <utility>

#include <SDL2pp/Font.hh>

#include "../../../config/constants.h"

struct FontKey {
    int size;
    std::string type;

    bool operator<(const FontKey& other) const {
        return std::tie(size, type) < std::tie(other.size, other.type);
    }
};

class FontManager {
private:
    std::map<FontKey, std::unique_ptr<SDL2pp::Font>> fonts;

public:
    FontManager();

    SDL2pp::Font& get_font(const FontKey& key);
};

#endif
