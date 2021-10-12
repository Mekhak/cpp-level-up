#include <cassert>
#include "thread_pool.h"
#include "utils.h"

thread_pool::thread_pool(size_t num_of_threads)
{
    m_stop = false;

    m_queue_cv = PTHREAD_COND_INITIALIZER;
    m_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

    m_workers.reserve(num_of_threads);

    // create threads in idle state
    for (int i = 0; i < num_of_threads; ++i) {
        pthread_t thread;
        int res = pthread_create(&thread, nullptr,
                                 (thread_pool::ThreadFuncType) &thread_pool::thread_function, this);
        utils::exit_on_error(res, "Cannot create thread.");

        m_workers.push_back(thread);
    }
}

void* thread_pool::thread_function(void *)
{
    while (true) {
        int res = pthread_mutex_lock(&m_queue_mutex);
        utils::exit_on_error(res, "Cannot lock mutex.");

        while (!m_stop && m_tasks.empty()) {
            utils::safe_print("waiting...");
            res = pthread_cond_wait(&m_queue_cv, &m_queue_mutex);
            utils::exit_on_error(res, "Cannot wait vor cond variable.");
        }

//        utils::safe_print("tasks.size(): " + std::to_string(m_tasks.size()));
//        utils::safe_print("workers.size(): " + std::to_string(m_workers.size()));

        // thread should exit if thread pool is stoped and all the tasks are executed
        utils::safe_print("m_stop: " + std::to_string(m_stop)
                          + "  m_tasks.size(): " + std::to_string(m_tasks.size()));
        if (m_stop && m_tasks.empty()) {
            utils::safe_print("exiting thread");
            return nullptr;
        }

        // queue must be non-empty here
        assert(("Attempt to read from an empty message queue", m_tasks.size() > 0));

        thread_pool::ThreadFuncType task = m_tasks.front();
        m_tasks.pop();

        res = pthread_mutex_unlock(&m_queue_mutex);
        utils::exit_on_error(res, "Cannot unlock mutex.");

        // execute the task
        // TODO: think a mechanizm to pass parameters for the registered/added functions
        task(nullptr);
    }
}

void* thread_pool::notifier(void*)
{
//    while ()

//    int res = pthread_mutex_lock(&m_queue_mutex);
//    utils::exit_on_error(res, "Cannot lock mutex.");

}

void thread_pool::add_task(ThreadFuncType task)
{
    int res = pthread_mutex_lock(&m_queue_mutex);
    utils::exit_on_error(res, "Cannot lock mutex.");

    m_tasks.push(task);

    res = pthread_cond_signal(&m_queue_cv);
    utils::exit_on_error(res, "Cannot notify cond variable.");

    res = pthread_mutex_unlock(&m_queue_mutex);
    utils::exit_on_error(res, "Cannot unlock mutex.");
}

void thread_pool::stop()
{
    int res = pthread_mutex_lock(&m_queue_mutex);
    utils::exit_on_error(res, "Cannot lock mutex.");

    m_stop = true;

    res = pthread_cond_broadcast(&m_queue_cv);
    utils::exit_on_error(res, "Cannot notify cond variable.");

    res = pthread_mutex_unlock(&m_queue_mutex);
    utils::exit_on_error(res, "Cannot unlock mutex.");

    for (auto thread : m_workers) {
        res = pthread_join(thread, nullptr);
        utils::exit_on_error(res, "Cannot join thread.");
    }
}

size_t thread_pool::get_workers_size() const noexcept
{
    return m_workers.size();
}

size_t thread_pool::get_tasks_size() const noexcept
{
    return m_tasks.size();
}

thread_pool::~thread_pool()
{
    stop();
}
