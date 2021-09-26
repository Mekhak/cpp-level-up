#include <fcntl.h>
#include <unistd.h>
#include "receiver.h"
#include "utils.h"

receiver::receiver(const char* fifo)
    : m_fifo(fifo)
{
}

void receiver::read_result() const
{
    char buf[200];

    int fd = try_open_fifo();

    int res = read(fd, buf, sizeof (buf));
    utils::exit_on_error(res, "Cannot read from fifo.");

    std::cout << "Result: " << std::endl
              << buf << std::endl;
}

int receiver::try_open_fifo() const
{
    int fd;

    // try to open fifo periodically (rate: 1 trial/sec.)
    // the open will success once task manager's executor process will create the fifo
    while (true)
    {
        fd = open(m_fifo, O_RDONLY);

        if (fd == -1) {
            if (errno == ENOENT) {
                sleep(1);
            } else {
                utils::exit_on_error(fd, "Cannot open fifo.");
            }
        } else {
            break;
        }
    }

    return fd;
}
