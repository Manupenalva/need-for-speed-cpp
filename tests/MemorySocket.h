// “Socket” simple en memoria
#include <cstdint>
#include <cstring>
#include <vector>

#include "../common/SocketInterface.h"

class MemorySocket: public ISocket {
public:
    std::vector<uint8_t> buffer;

    int sendall(const void* data, unsigned int size) override {
        const uint8_t* bytes = static_cast<const uint8_t*>(data);
        buffer.insert(buffer.end(), bytes, bytes + size);
        return size;
    }

    int recvall(void* dest, unsigned int size) override {
        if (buffer.size() < size)
            return 0;
        std::memcpy(dest, buffer.data(), size);
        buffer.erase(buffer.begin(), buffer.begin() + size);
        return size;
    }

    void shutdown(int how) override {
        (void)how;
        // No hace nada en este socket de memoria
    }

    bool is_empty() const { return buffer.empty(); }

    bool is_stream_send_closed() const override { return false; }

    bool is_stream_recv_closed() const override { return false; }
};
