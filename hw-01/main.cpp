#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    const char* fname = "something.txt";

    std::cout << "Writing something in "
              << fname
              << " via std::cout ..."
              << std::endl;

    // close stdout
    close(1);

    int fd = open(fname, O_CREAT | O_WRONLY, S_IRWXU);

    if (fd == -1) {
        return errno;
    }

    std::cout << "some text ..." << std::endl;

    if (close(fd) == -1) {
        return errno;
    }

    return 0;
}
