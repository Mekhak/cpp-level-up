#include "client.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "utils.h"

const size_t client::s_port = 63103;
const char* client::s_ip = "127.0.0.1";

client::client()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    utils::exit_on_error(sock, "Cannot create socket.", -1);

    m_socket = sock;

    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(client::s_port);

    int res = inet_pton(AF_INET, client::s_ip, &addr.sin_addr);
    utils::exit_on_error(res, "Cannot conver ip string to ip addr.", 0, 1);

    res = connect(sock, (struct sockaddr *) &addr, sizeof addr);
    utils::exit_on_error(res, "Cannot connet socket.", -1);
}

void client::start() noexcept
{
    std::cout << "Type a message ...\n" << std::endl;

    int res = pthread_create(&m_listener, nullptr,
                             (client::ThreadFuncType) &client::listen, this);
    utils::exit_on_error(res, "Cannot create thread.");

    res = pthread_detach(m_listener);
    utils::exit_on_error(res, "Cannot detach thread.");

    while (true) {
        utils::safe_print("S:", false);

        std::string msg;
        std::getline(std::cin, msg);

        int res = write(m_socket, msg.c_str(), 256);
        utils::exit_on_error(res, "Cannot write to socket.", -1);
    }
}

void* client::listen(void *)
{
    while (true) {
        char buf[256];

        int res = read(m_socket, buf, 256);
        utils::exit_on_error(res, "Cannot read from socket.", -1);

        utils::safe_print("\nR: ", false);
        utils::safe_print(buf);

        utils::safe_print("S:", false);
    }
}
