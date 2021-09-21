#ifndef TEST_EXECUTOR_H
#define TEST_EXECUTOR_H

#include <vector>
#include "mylib.h"

class test_executor
{
public:
    test_executor(const char* output_fname);
    ~test_executor();

    void run();

private:
    void report(const std::string& msg);

private:
    int m_output_file;

    using function_type = decltype (&mylib::func1);
    std::vector<function_type> m_functions;

    static const char* SuccessMessage;
    static const char* FailureMessage;
    static const char* UnresolvedMessage;

    static int MaxMessageLength;
};

#endif // TEST_EXECUTOR_H
