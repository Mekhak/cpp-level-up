#include <sys/shm.h>
//#include <stdio.h>
#include <string.h>
#include "shared_object.h"
#include "utils.h"

shared_object* shared_object::s_instance = nullptr;

shared_object::shared_object()
    : m_constructed(true)
{
    // initialize all fifo names with a special value -1 which indicates empty value
    for (size_t i = 0; i < s_max_num_of_processes; ++i) {
        m_fifo_names[i] = -1;
    }
}

shared_object::~shared_object()
{
}

shared_object& shared_object::get_instance()
{
    if (s_instance == nullptr) {
        int id = shmget(s_id, sizeof (shared_object), IPC_CREAT | 0666);
        utils::exit_on_error(id, "Cannot get a shared memory.", -1);

        void* raw_mem = shmat(id, nullptr, 0);
        s_instance = static_cast<shared_object*>(raw_mem);

        // construct object on allocated memory
        if (s_instance->m_constructed != true) {
            new (s_instance) shared_object;
        }
    }

    return *s_instance;
}

void shared_object::register_callback(pid_t fifo_name) noexcept
{
    // register at the first "free" index (-1)
    for (size_t i = 0; i < s_max_num_of_processes; ++i) {
        if (m_fifo_names[i] == -1) {
            m_fifo_names[i] = fifo_name;
            break;
        }
    }
}

void shared_object::unregister_callback(pid_t fifo_name) noexcept
{
    for (size_t i = 0; i < s_max_num_of_processes; ++i) {
        if (m_fifo_names[i] == fifo_name) {
            m_fifo_names[i] = -1;
            break;
        }
    }
}

void shared_object::set_message(const char *msg)
{
    strncpy(m_message, msg, s_max_msg_length);

    // write the updated message in all the fifos
    for (size_t i = 0; i < s_max_num_of_processes; ++i) {
        if (m_fifo_names[i] == -1) {
            continue;
        }

        std::string fifo_name = "/tmp/" + std::to_string(m_fifo_names[i]);

        int fd = open(fifo_name.c_str(), O_WRONLY | O_NONBLOCK);

        if (fd == -1 && (errno == ENOENT || errno == ENXIO)) {
            // the fifo doesn't exist. Unregistering from the observer list
            unlink(fifo_name.c_str());
            m_fifo_names[i] = -1;
            continue;
        }

        utils::exit_on_error(fd, "Cannot open fifo!!!!!.", -1);

        int res = write(fd, m_message, s_max_msg_length);
        utils::exit_on_error(res, "Cannot write into fifo.", -1);
    }
}

const char* shared_object::get_message() const noexcept
{
    return m_message;
}
