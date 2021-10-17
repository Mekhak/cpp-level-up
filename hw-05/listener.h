#ifndef LISTENER_H
#define LISTENER_H

#include <string>
#include <pthread.h>

class listener
{
public:
    listener();
    ~listener();

public:
    void start();

private:
    using ThreadFuncType = void* (*)(void*);
    void* listen(void*);

private:
    pthread_t m_listener_thread;
    std::string m_fifo_name;
};

#endif // LISTENER_H
