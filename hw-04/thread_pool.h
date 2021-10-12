#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <pthread.h>

class thread_pool
{
public:
    thread_pool(size_t num_of_workers);
    ~thread_pool();

    thread_pool(const thread_pool&) = delete;
    thread_pool& operator=(const thread_pool&) = delete;

    using ThreadFuncType = void* (*) (void*);

    void add_task(ThreadFuncType task);
    void stop();

    size_t get_workers_size() const noexcept;
    size_t get_tasks_size() const noexcept;

private:
    void* thread_function(void*);
    void* notifier(void*);

private:
    pthread_t m_notifier_thread; // thread that periodically notifies the workers

    std::vector<pthread_t> m_workers;
    std::queue<ThreadFuncType> m_tasks;

    bool m_stop;
    bool m_stop_notify;

    pthread_cond_t m_queue_cv;
    pthread_mutex_t m_queue_mutex;
};

#endif // THREAD_POOL_H
