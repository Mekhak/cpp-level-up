#ifndef SEMAPHORE_WRAPPER_H
#define SEMAPHORE_WRAPPER_H

#include <semaphore.h>
#include <fcntl.h>
#include "utils.h"

namespace {

const char* sem_name = "semaphore_name";

} // unnamed

class semaphore_wrp {
private:
    semaphore_wrp()
    {
        m_sem = sem_open(sem_name, O_CREAT, 0666, s_value);
        if (m_sem == SEM_FAILED) {
            utils::exit_on_error(-1, "Cannot open semaphore.", -1);
        }
    }

public:
    semaphore_wrp(const semaphore_wrp&) = delete;
    semaphore_wrp(semaphore_wrp&&) = delete;
    semaphore_wrp& operator=(const semaphore_wrp&) = delete;
    semaphore_wrp& operator=(semaphore_wrp&&) = delete;

    static semaphore_wrp& get_instance()
    {
        static semaphore_wrp instance;
        return instance;
    }

    void wait() noexcept
    {
        int res = sem_wait(m_sem);
        utils::exit_on_error(res, "Cannot wait semaphore.", -1);
    }

    void post() noexcept
    {
        int res = sem_post(m_sem);
        utils::exit_on_error(res, "Cannot post semaphore.", -1);
    }

    ~semaphore_wrp()
    {
        int res = sem_unlink(sem_name);
        utils::exit_on_error(res, "Cannot unlink semaphore.", -1);

        res = sem_close(m_sem);
        utils::exit_on_error(res, "Cannot close semaphore.", -1);
    }

private:
    sem_t* m_sem;
    static const int s_value = 1;
    static const std::string* s_name;
};

#endif // SEMAPHORE_WRAPPER_H
