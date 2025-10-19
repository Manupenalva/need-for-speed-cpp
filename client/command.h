#ifndef COMMAND_H
#define COMMAND_H

#include <cstdint>
#include <vector>

enum class CommandType : uint8_t {
    
};

class Command {
private:
    std::vector<uint8_t> eventos;

public:
    Command() = default;

    // Agrega un evento a la lista de eventos
    void add_event(uint8_t event) {
        eventos.push_back(event);
    }

    // Devuelve la lista de eventos ya serializados para enviar por protocolo al servidor
    const std::vector<uint8_t>& get_events() const {
        return eventos;
    }
    
    // Se usa para ver si la lista de eventos esta vacia, en cuyo caso no se envia nada
    bool is_empty() const {
        return eventos.empty();
    }
};

#endif // COMMAND_H