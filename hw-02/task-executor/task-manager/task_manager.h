#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <vector>
#include <string>

class task_manager
{
public:
    task_manager(const char* input_fifo);
    ~task_manager();

    void run();

private:
    std::vector<std::string> parse_message(const std::string& msg) const;
    void handle_command(const std::string& output_fifo, const std::string& cmd);

private:
    const char* m_input_fifo;
};

#endif // TASK_MANAGER_H
