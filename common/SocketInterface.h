#ifndef COMMON_SOCKETINTERFACE_H
#define COMMON_SOCKETINTERFACE_H
#include <cstddef>

class ISocket {
public:
    virtual ~ISocket() = default;

    virtual int sendall(const void* data, unsigned int sz) = 0;
    virtual int recvall(void* data, unsigned int sz) = 0;
    virtual void shutdown(int how) = 0;

    virtual bool is_stream_send_closed() const = 0;
    virtual bool is_stream_recv_closed() const = 0;
};
#endif
