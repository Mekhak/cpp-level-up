#ifndef MYLIB_H
#define MYLIB_H

#include <iostream>

namespace mylib
{
    const int NumOfFunctions = 3;

    inline bool func1()
    {
        std::cout << "Starting func1..." << std::endl;

        // success
        return true;
    }

    inline bool func2()
    {
        std::cout << "Starting func2..." << std::endl;

        // fail
        return false;
    }

    inline bool func3()
    {
        std::cout << "Starting func3..." << std::endl;
        abort();

        // unresolved
        return true;
    }

} // namespace mylib

#endif // MYLIB_H
