#include "test_executor.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Usage: `test_executor filename`" << std::endl;
        return 0;
    }

    test_executor t(argv[1]);
    t.run();

    std::cout << "The report is generated in a file with the specified filename!" << std::endl;

    return 0;
}
