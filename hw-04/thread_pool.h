#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <pthread.h>

class thread_pool
{
public:
    thread_pool(size_t num_of_workers);
    ~thread_pool() = default;

    thread_pool(const thread_pool&) = delete;
    thread_pool& operator=(const thread_pool&) = delete;

    using ThreadFuncType = void* (*) (void*);

    void add_task(ThreadFuncType task);
    void stop(); // finalize the tasks and exit

    size_t get_workers_size() const noexcept;
    size_t get_tasks_size() const noexcept;

private:
    void* thread_function(void*); // all threads run this function

private:
    std::vector<pthread_t> m_workers;
    std::queue<ThreadFuncType> m_tasks;

    bool m_stop;

    pthread_cond_t m_queue_cv;
    pthread_mutex_t m_queue_mutex;
};

#endif // THREAD_POOL_H
