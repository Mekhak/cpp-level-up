#ifndef RECEIVER_H
#define RECEIVER_H

class receiver
{
public:
    receiver(const char* fifo);

    void read_result() const;

private:
    int try_open_fifo() const;

private:
    const char* m_fifo;
};

#endif // RECEIVER_H
