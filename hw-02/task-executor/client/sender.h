#ifndef SENDER_H
#define SENDER_H

#include <unistd.h>

class sender
{
public:
    sender(const char* fifo);

    void send(const char* output_fifo, const char* cmd);

private:
    const char* m_input_fifo;
};

#endif // SENDER_H
