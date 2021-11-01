#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <vector>
#include <pthread.h>
#include "message_queue.h"


class server
{
public:
    server();
    ~server();

    void start() noexcept;

    void broadcast_message(const std::string&); // callback function to be called by message_queue

    void add_message(const std::string& msg);

    struct arg_wrapper
    {
        server* srvr;
        int socket;
    };

private:
    std::vector<pthread_t> m_workers; // workers are listening to the clients and adding messages in the message queue
    message_queue m_msg_queue; // messages

    std::vector<int> m_client_sockets;
    std::vector<arg_wrapper*> m_args;

    static const size_t s_port;
    static const char* s_ip;
    static const size_t s_max_num_of_clients;
};

#endif // SERVER_H
