#ifndef RESULT_INFO_H
#define RESULT_INFO_H

#include <cstdint>
#include <string>

struct ResultInfo {
    uint16_t id;
    float time;
    float penalization_time;
    std::string name;

    ResultInfo(uint16_t id_, float time_, float penalization_time_, const std::string& name_):
            id(id_), time(time_), penalization_time(penalization_time_), name(name_) {}

    ResultInfo(): id(0), time(0.0f), penalization_time(0.0f), name("") {}
};
#endif
