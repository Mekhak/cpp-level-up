#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <unistd.h>
#include <pthread.h>

namespace  {

pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

} // unnamed

namespace utils {

inline void exit_on_error(int err, const char* msg,
                          int failure_code = 0, // zero means that function returns `normal` on success
                          int normal = 0)
{
    if ((failure_code == 0 && err != normal) ||
            (failure_code != 0 && err == failure_code)) {

        std::cerr << "System Error: "
                  << msg << std::endl;

        exit(EXIT_FAILURE);
    }
}

inline std::string get_fifo_name()
{
    return "/tmp/" + std::to_string(getpid());
}

template <typename T>
void safe_print(const T& msg)
{
    int res = pthread_mutex_lock(&print_mutex);
    exit_on_error(res, "Cannot lock mutex.");

    std::cout << msg << std::endl;

    pthread_mutex_unlock(&print_mutex);
    exit_on_error(res, "Cannot unlock mutex.");
}

} // utils

#endif // UTILS_H
