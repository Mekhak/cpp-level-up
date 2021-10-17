#include "shared_object.h"
#include "listener.h"
#include "utils.h"
#include "semaphore_wrapper.h"

int main(int argc, char *argv[])
{
    listener l;
    l.start();

    // each process modifies the shared resource 10 times with some fixed rate
    // while handling the updates made by the other processes
    for (size_t i = 0; i < 10; ++i) {
        std::string msg = "message-" + std::to_string(i);

        std::string log = std::to_string(getpid());
        log += ": set message `";
        log += msg;
        log += "`";

        utils::safe_print(log);

        semaphore_wrp::get_instance().wait();
        shared_object::get_instance().set_message(msg.c_str());
        semaphore_wrp::get_instance().post();

        sleep(1);
    }

    return 0;
}
