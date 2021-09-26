#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include "task_manager.h"
#include "utils.h"

task_manager::task_manager(const char* input_fifo)
    : m_input_fifo(input_fifo)
{
    int res = mkfifo(m_input_fifo, 0666);

    if (res == -1) {
        if (errno != EEXIST) {
            utils::exit_on_error(res, "Cannot create fifo.");
        }
    }
}

task_manager::~task_manager()
{
    int res = unlink(m_input_fifo);
    utils::exit_on_error(res, "Cannot unlink fifo.");
}

void task_manager::run()
{
    std::cout << "Task manager started!\n" << std::endl;

    char msg[40];

    while (true) {
        std::cout << "Waiting for a command to execute ..." << std::endl;

        // read output fifo name
        int fd = open(m_input_fifo, O_RDONLY);
        utils::exit_on_error(fd, "Cannot open fifo.");

        int num_bytes = read(fd, msg, sizeof(msg));
        utils::exit_on_error(num_bytes, "Cannot read from fifo.");

        const auto msgs = parse_message(msg);
        const std::string output_fifo = msgs[0];
        const std::string cmd = msgs[1];

        std::cout << "Command received: " << cmd << std::endl << std::endl;

        int res = close(fd);
        utils::exit_on_error(res, "Cannot close fifo.");

        pid_t pid = fork();
        utils::exit_on_error(pid, "Cannot create process.");

        if (pid == 0) {
            handle_command(output_fifo, cmd);
            break;
        }
    }
}

void task_manager::handle_command(const std::string& output_fifo, const std::string& cmd)
{
    int res = mkfifo(output_fifo.c_str(), 0666);

    if (res == -1) {
        if (errno != EEXIST) {
            utils::exit_on_error(res, "Cannot create fifo.");
        }
    }

    // dup fifo file descriptor with standard out to
    // therefore, command execution result will be writen into fifo
    int fd = open(output_fifo.c_str(), O_WRONLY);
    utils::exit_on_error(fd, "Cannot open fifo.");
    dup2(fd, STDOUT_FILENO);
    close(fd);

    // execute the command
    execl(("/bin/" + cmd).c_str(), cmd.c_str(), nullptr);
    std::cerr << "Error: exec failed." << std::endl;
}

std::vector<std::string> task_manager::parse_message(const std::string &msg) const
{
    auto separator = std::find(msg.begin(), msg.end(), '*');

    size_t dist = std::distance(msg.begin(), separator);

    return {msg.substr(0, dist), msg.substr(dist + 1, msg.length())};
}
