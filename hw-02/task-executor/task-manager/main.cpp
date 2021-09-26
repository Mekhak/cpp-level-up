#include <iostream>
#include "task_manager.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Usage: `task-manager input-fifo-name`." << std::endl;
        return 0;
    }

    task_manager mngr(argv[1]);
    mngr.run();

    return 0;
}
