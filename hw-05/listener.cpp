#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "listener.h"
#include "utils.h"
#include "shared_object.h"
#include "semaphore_wrapper.h"

listener::listener()
{
    m_fifo_name = utils::get_fifo_name();

    // create fifo named with process id
    int res = mkfifo(m_fifo_name.c_str(), 0666);

    int fd = open(m_fifo_name.c_str(), O_RDONLY | O_NONBLOCK);
    utils::exit_on_error(fd, "Cannot open fifo.", -1);

    if (res == -1 && errno != EEXIST) {
        utils::exit_on_error(res, "Cannot create fifo.", -1);
    }

    // register in shared object
    semaphore_wrp::get_instance().wait();
    shared_object::get_instance().register_callback(getpid());
    semaphore_wrp::get_instance().post();
}

void listener::start()
{
    // fire the listener thread
    int res = pthread_create(&m_listener_thread, nullptr,
                             (listener::ThreadFuncType) &listener::listen, this);
    utils::exit_on_error(res, "Cannot create thread.");
}

void* listener::listen(void *)
{
    char msg[shared_object::s_max_msg_length];

    while (true) {
        // try to read from the fifo
        int fd = open(m_fifo_name.c_str(), O_RDONLY);
        utils::exit_on_error(fd, "Cannot open fifo.", -1);

        // check for update/notification
        int res = read(fd, msg, sizeof(msg));
        utils::exit_on_error(res, "Cannot read from fifo.", -1);

        std::string log(std::to_string(getpid()));
        log += ": notified. `";
        log += std::string(msg);
        log += "`.";

        utils::safe_print(log);
    }
}

listener::~listener()
{
    int res = pthread_join(m_listener_thread, nullptr);
    utils::exit_on_error(res, "Cannot join thread.");

    res = unlink(m_fifo_name.c_str());
    utils::exit_on_error(res, "Cannot unlink fifo.", -1);

    semaphore_wrp::get_instance().wait();
    shared_object::get_instance().unregister_callback(getpid());
    semaphore_wrp::get_instance().post();
}
