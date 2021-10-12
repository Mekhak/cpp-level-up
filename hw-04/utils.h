#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <pthread.h>

namespace  {

pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

} // unnamed


namespace utils {

inline void exit_on_error(int err, const char* msg, int normal = 0)
{
    if (err != normal) {
        std::cerr << "System Error: "
                  << msg << std::endl;

        exit(EXIT_FAILURE);
    }
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
