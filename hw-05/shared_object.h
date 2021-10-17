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
    ~shared_object() = default;

public:
    static shared_object& get_instance();

    // registering fifo names (the process id of the observer) for "callback" notification
    void register_callback(pid_t fifo_name) noexcept;
    void unregister_callback(pid_t fifo_name) noexcept;

    void set_message(const char* msg);
    const char* get_message() const noexcept;

    static const size_t s_max_msg_length = 15;

private:
    static shared_object* s_instance; // singleton instance

    char m_message[s_max_msg_length]; // some intrinsic value of the shared object

    static const size_t s_max_num_of_processes = 10; // max number of the observer processes
    pid_t m_fifo_names[s_max_num_of_processes]; // list of the observer processes fifo names

    bool m_constructed; // specifies whether the shared object is initialized or not, i.e, if constructor is called on the shared memory

    static const int s_id = 3; // a parameter for shmget function
};

#endif // SHARED_OBJECT_H
