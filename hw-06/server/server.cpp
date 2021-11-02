#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include <pthread.h>

#include "utils.h"

namespace {

// listen for client given a socket
void* listen_to_client(void* args)
{
    server::arg_wrapper* wrp = static_cast<server::arg_wrapper*>(args);

    server* srvr = wrp->srvr;
    int socket = wrp->socket;

    while (true) {
        char buf[256];

        int res = read(socket, buf, 256);
        utils::exit_on_error(res, "Cannot read from socket.", -1);

        srvr->add_message(buf);
    }
}

} // unnamed

const size_t server::s_port = 63103;
const char* server::s_ip = "127.0.0.1";
const size_t server::s_max_num_of_clients = 10;

server::server()
{
    m_msg_queue.register_observer(this);
}

void server::start() noexcept
{
    int server = socket(AF_INET, SOCK_STREAM, 0);
    utils::exit_on_error(server, "Cannot create socket.", -1);

    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(server::s_port);

    int res = inet_pton(AF_INET, server::s_ip, &addr.sin_addr);
    utils::exit_on_error(res, "Cannot conver ip string to ip addr.", 0, 1);

    res = bind(server, reinterpret_cast<sockaddr*>(&addr), sizeof (addr));
    utils::exit_on_error(res, "Cannot bind address to socket.");

    res = listen(server, server::s_max_num_of_clients);
    utils::exit_on_error(res, "Cannot listen to socket.");

    std::cout << "Server started." << std::endl;

    while (true) {
        socklen_t addr_len = 0;

        int client_socket = accept(server, reinterpret_cast<sockaddr*>(&addr), &addr_len);
        utils::exit_on_error(client_socket, "Cannot accept new connection.", -1);

        m_client_sockets.push_back(client_socket);

        pthread_t worker;

        arg_wrapper* args = new arg_wrapper;
        args->srvr = this;
        args->socket = client_socket;

        int res = pthread_create(&worker, nullptr, listen_to_client, args);
        utils::exit_on_error(res, "Cannot create thread.");

        res = pthread_detach(worker);
        utils::exit_on_error(res, "Cannot detach thread.");
    }
}

void server::broadcast_message(const std::string& msg)
{
    for (auto socket : m_client_sockets) {
        write(socket, msg.c_str(), 256);
    }
}

void server::add_message(const std::string& msg)
{
    m_msg_queue.add_message(msg);
}

server::~server()
{
    for (auto& arg : m_args) {
        delete arg;
        arg = nullptr;
    }
}
