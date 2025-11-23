#ifndef RESULT_INFO_H
#define RESULT_INFO_H

#include <cstdint>
#include <string>

struct ResultInfo {
    uint16_t id;
    float time;
    float penalization_time;
    std::string name;
};
#endif
