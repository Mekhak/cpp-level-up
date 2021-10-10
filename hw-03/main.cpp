#include <pthread.h>
#include <unistd.h>
#include "message_queue.h"
#include "utils.h"

void h1(std::string msg)
{
    utils::safe_print("Handler h1: " + msg);
}

void h2(std::string msg)
{
    utils::safe_print("Handler h2: " + msg);
}

void h3(std::string msg)
{
    utils::safe_print("Handler h3: " + msg);
}

int main(int argc, char *argv[])
{
    message_queue msgq;

    // register handlers
    msgq.register_handler(h1);
    msgq.register_handler(h2);
    msgq.register_handler(h3);

    // add messages
    for (int i = 0; i < 4; ++i) {
        std::string msg = "message_" + std::to_string(i + 1);
        msgq.add_message(msg);

        utils::safe_print("Message added: " + msg);
    }

    sleep(2);

    return 0;
}
