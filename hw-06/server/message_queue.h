#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <string>
#include <queue>
#include <vector>

class server;

class message_queue
{
public:
    message_queue();
    ~message_queue();

    using ThreadFuncType = void* (*) (void*);
    using HandlerType = void (*) (const std::string&);

    void add_message(const std::string& msg);
    void register_observer(server* observer) noexcept;

private:
    void* start(void*); // function of the scheduler thread

private:
    std::queue<std::string> m_queue;
    std::vector<server*> m_observers;

    pthread_cond_t m_queue_cv; // cond variable for waiting for a new item in queue
    pthread_mutex_t m_cv_mutex; // mutex for cond var

    pthread_t m_scheduler_thread;
};

#endif // MESSAGE_QUEUE_H
