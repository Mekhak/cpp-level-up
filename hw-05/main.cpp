#include "shared_object.h"
#include "listener.h"
#include "utils.h"

int main(int argc, char *argv[])
{
//    shared_object& obj = shared_object::get_instance();
//    obj.set_message("13100");

    listener l;
    l.start();

    for (size_t i = 0; i < 10; ++i) {
        std::string msg = "message-" + std::to_string(i);

        if (argc == 2) {
            msg += "****";
        }

        std::string log = std::to_string(getpid());
        log += ": set message `";
        log += msg;
        log += "`";

        utils::safe_print(log);

        shared_object::get_instance().set_message(msg.c_str());

        sleep(1);
    }

    return 0;
}
