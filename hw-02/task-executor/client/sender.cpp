#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include "sender.h"
#include "utils.h"

sender::sender(const char* fifo)
    : m_input_fifo(fifo)
{
}

void sender::send(const char* output_fifo, const char *cmd)
{
    // write cmd into fifo
    int fd = open(m_input_fifo, O_WRONLY);
    utils::exit_on_error(fd, "Cannot open fifo.");

    // concat the output_fifo and cmd strings with a special symbol *
    std::string msg(output_fifo);
    msg += "*";
    msg += std::string(cmd);

    int res = write(fd, msg.c_str(), msg.size() + 1);
    utils::exit_on_error(res, "Cannot write into fifo.");

    res = close(fd);
    utils::exit_on_error(res, "Cannot close fifo.");

    std::cout << "Command " << cmd << " sent." << std::endl;
}
