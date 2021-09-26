#include <iostream>
#include "sender.h"
#include "receiver.h"

int main(int argc, char *argv[])
{
    if (argc != 4) {
        std::cout << "Usage: `client send-fifo receive-fifo cmd`." << std::endl;
        return 0;
    }

    sender s(argv[1]);
    s.send(argv[2], argv[3]);

    receiver r(argv[2]);
    r.read_result();

    return 0;
}
