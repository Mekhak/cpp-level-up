#ifndef UTILS_H
#define UTILS_H

#include <iostream>

namespace utils {

inline void exit_on_error(int err, const char* msg)
{
    if (err == -1) {
        std::cerr << "System Error: "
                  << msg << std::endl;

        exit(EXIT_FAILURE);
    }
}

} // namespace utils

#endif // UTILS_H
