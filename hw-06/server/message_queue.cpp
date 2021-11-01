#include <cassert>
#include "message_queue.h"
#include "utils.h"
#include "server.h"

message_queue::message_queue()
{
    m_queue_cv = PTHREAD_COND_INITIALIZER;
    m_cv_mutex = PTHREAD_MUTEX_INITIALIZER;

    // start the scheduler thread

    int res = pthread_create(&m_scheduler_thread, nullptr,
                             (message_queue::ThreadFuncType) &message_queue::start, this);
    utils::exit_on_error(res, "Cannot create thread.");

    res = pthread_detach(m_scheduler_thread);
    utils::exit_on_error(res, "Cannot detach thread.");
}

void message_queue::add_message(const std::string &msg)
{
    int res = pthread_mutex_lock(&m_cv_mutex);
    utils::exit_on_error(res, "Cannot lock mutex.");

    // add the new message to the queue
    m_queue.push(msg);

    // notify the scheduler thread that queue is not empty
    res = pthread_cond_signal(&m_queue_cv);

    res = pthread_mutex_unlock(&m_cv_mutex);
    utils::exit_on_error(res, "Cannot unlock mutex.");
}

void message_queue::register_observer(server* observer) noexcept
{
    m_observers.push_back(observer);
}

void* message_queue::start(void*)
{
    while (true) {
        // create and fire a thread for each registered function

        int res = pthread_mutex_lock(&m_cv_mutex);
        utils::exit_on_error(res, "Cannot lock mutex.");

        while (m_queue.empty()) {
            res = pthread_cond_wait(&m_queue_cv, &m_cv_mutex);
            utils::exit_on_error(res, "Cannot wait for cond var.");
        }

        // queue must be non-empty here
        assert(("Attempt to read from an empty message queue", m_queue.size() > 0));

        const std::string& msg = m_queue.front();

        for (auto observer : m_observers) {
            observer->broadcast_message(msg);
        }

        m_queue.pop();

        res = pthread_mutex_unlock(&m_cv_mutex);
        utils::exit_on_error(res, "Cannot unlock mutex.");
    }
}

message_queue::~message_queue()
{

}
