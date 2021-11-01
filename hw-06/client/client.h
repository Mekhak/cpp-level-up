#ifndef CLIENT_H
#define CLIENT_H

#include <pthread.h>

class client
{
public:
    client();
    void start() noexcept;

private:
    void* listen(void*);

    using ThreadFuncType = void* (*)(void*);

private:
    int m_socket;
    pthread_t m_listener;

    static const size_t s_port;
    static const char* s_ip;
};

#endif // CLIENT_H
