#include <cassert>
#include "message_queue.h"
#include "utils.h"

namespace {

// a wrapper function that all threads should call
// the argument arg holds both the callback function and its argument (std::string)
void* callback_wrapper(void* arg)
{
    message_queue::arg_wrapper* wrp = static_cast<message_queue::arg_wrapper*>(arg);

    auto callback = wrp->callback;
    auto msg = wrp->msg;

    callback(msg);

    return nullptr;
}

} // unnamed

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

void message_queue::register_handler(HandlerType handler) noexcept
{
    m_handlers.push_back(handler);
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

        for (auto callback : m_handlers) {
            pthread_t thread;

            arg_wrapper* args = new arg_wrapper(msg, callback);
            m_arg_wrappers.push_back(args);

            int res = pthread_create(&thread, nullptr, callback_wrapper, (void*)(args));
            utils::exit_on_error(res, "Cannot create thread.");

            res = pthread_detach(thread);
        }

        m_queue.pop();

        res = pthread_mutex_unlock(&m_cv_mutex);
        utils::exit_on_error(res, "Cannot unlock mutex.");
    }
}

message_queue::~message_queue()
{
    for (auto item : m_arg_wrappers) {
        delete item;
        item = nullptr;
    }
}
