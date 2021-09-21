#include "test_executor.h"
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

const char* test_executor::SuccessMessage = "Success";
const char* test_executor::FailureMessage = "Failure";
const char* test_executor::UnresolvedMessage = "Unresolved";

int test_executor::MaxMessageLength;

test_executor::test_executor(const char* output_fname)
{
    int fd = open(output_fname, O_CREAT | O_WRONLY, S_IRWXU);

    if (fd == -1) {
        std::cerr << "System Error: cannot create file! Error number: "
                  << std::to_string(errno);

        exit(EXIT_FAILURE);
    }

    m_functions.reserve(mylib::NumOfFunctions);

    m_functions.push_back(&mylib::func1);
    m_functions.push_back(&mylib::func2);
    m_functions.push_back(&mylib::func3);

    MaxMessageLength = std::max(sizeof (SuccessMessage), sizeof (FailureMessage));
}

test_executor::~test_executor()
{
    close(m_output_file);
}

void test_executor::run()
{
    for (size_t i = 0; i < m_functions.size(); ++i) {

        int fds[2];

        int ret_val = pipe(fds);

        if (ret_val == -1) {
            close(fds[0]);
            close(fds[1]);

            std::cerr << "System Error: cannot create pipe! Error number: "
                      << std::to_string(errno);
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();

        if (pid == -1) {
            close(fds[0]);
            close(fds[1]);

            std::cerr << "System Error: cannot create process! Error number: "
                      << std::to_string(errno);

            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // child process writes into the pipe, threfore closing the read descriptor
            close(fds[0]);

            // execute the function
            bool ok = m_functions[i]();

            const char* msg;
            int length;

            if (ok) {
                msg = test_executor::SuccessMessage;
                length = sizeof (test_executor::SuccessMessage);
            } else {
                msg = test_executor::FailureMessage;
                length = sizeof (test_executor::FailureMessage);
            }

            int res = write(fds[1], msg, length);

            if (res == -1) {
                close(fds[1]);

                std::cerr << "System Error: Cannot write into file! Error number: "
                          << std::to_string(errno);

                exit(EXIT_FAILURE);
            }

            close(fds[1]);

            // the child process exited successfully
            exit(EXIT_SUCCESS);

        } else {
            // parent process reads from the pipe, threfore closing the write descriptor
            close(fds[1]);

            // wait for child process
            int wstatus;

            int res = waitpid(pid, &wstatus, WUNTRACED | WCONTINUED);

            if (res == -1) {
                close(fds[0]);

                std::cerr << "System Error: Cannot wait for a child process! Error number: "
                          << std::to_string(errno);

                exit(EXIT_FAILURE);
            }

            bool ok = true;

            if (WIFEXITED(wstatus)) {
                int exit_status = WEXITSTATUS(wstatus);

                if (exit_status == 0) {
                    char msg[MaxMessageLength];

                    read(fds[0], msg, MaxMessageLength);

                    std::string message = "Function " + std::to_string(i + 1) + ": " + msg;

                    report(message);
                } else {
                    ok = false;
                }

            } else if (WIFSIGNALED(wstatus)) {
                ok = false;
            }

            if (!ok) {
                // child process exited with non zero exit code
                // or throwed a signal

                std::string message = "Function " + std::to_string(i + 1) + ": " +
                                      UnresolvedMessage;

                report(message);
            }

            close(fds[0]);
        }
    }
}

void test_executor::report(const std::string& msg)
{
    int res = write(m_output_file, msg.c_str(), msg.size());

    if (res == -1) {
        std::cerr << "System Error: cannot write into file! Error number: "
                  << std::to_string(errno);

        exit(EXIT_FAILURE);
    }

    const char separator[] = "\n--------------------\n";

    // this write is not critically important, therefore ignoring the possible failure
    write(m_output_file, separator, sizeof(separator));
}
