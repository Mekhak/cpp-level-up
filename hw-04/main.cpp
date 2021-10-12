#include <unistd.h>
#include "thread_pool.h"
#include "utils.h"

void* task1(void*)
{
    for (int i = 0; i < 10; ++i) {
        utils::safe_print("task1: " + std::to_string(i));
        usleep(100000);
    }
}

void* task2(void*)
{
    for (int i = 10; i < 20; ++i) {
        utils::safe_print("task2: " + std::to_string(i));
        usleep(100000);
    }
}

void* task3(void*)
{
    for (int i = 20; i < 30; ++i) {
        utils::safe_print("task3: " + std::to_string(i));
        usleep(100000);
    }
}

void* task4(void*)
{
    for (int i = 30; i < 40; ++i) {
        utils::safe_print("task4: " + std::to_string(i));
        usleep(100000);
    }
}

void* task5(void*)
{
    for (int i = 40; i < 50; ++i) {
        utils::safe_print("task5: " + std::to_string(i));
        usleep(100000);
    }
}

int main(int argc, char *argv[])
{
    thread_pool pool(3);

    pool.add_task(task1);
    pool.add_task(task2);
    pool.add_task(task3);
    pool.add_task(task4);
    pool.add_task(task5);

    utils::safe_print("Number of tasks: " + std::to_string(pool.get_tasks_size()));
    utils::safe_print("Number of workers: " + std::to_string(pool.get_workers_size()));

    pool.stop();

    return 0;
}
