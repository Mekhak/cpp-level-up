#ifndef SHARED_OBJECT_H
#define SHARED_OBJECT_H

#include <fcntl.h>

class shared_object
{
private:
    shared_object();

public:
    shared_object(const shared_object&) = delete;
    shared_object(shared_object&&) = delete;
    shared_object& operator=(const shared_object&) = delete;
    shared_object& operator=(shared_object&&) = delete;
    ~shared_object();

public:
    static shared_object& get_instance();

    void register_callback(pid_t fifo_name) noexcept;
    void unregister_callback(pid_t fifo_name) noexcept;

    void set_message(const char* msg);
    const char* get_message() const noexcept;

    static const size_t s_max_msg_length = 15;

private:
    static shared_object* s_instance;

    char m_message[s_max_msg_length];

    static const size_t s_max_num_of_processes = 10;
    pid_t m_fifo_names[s_max_num_of_processes];

    bool m_constructed;

    static const int s_id = 3;
};

#endif // SHARED_OBJECT_H
