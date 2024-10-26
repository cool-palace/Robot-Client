#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <thread>
#include <chrono>
#include <ctime>

using boost::asio::ip::tcp;

class Client {
public:
    Client(boost::asio::io_context&, const std::string&, int);
    void start();

private:
    tcp::socket socket_;
    std::string server_ip_;
    int server_port_;

    void connect();
    void listen_for_commands();
    void send_command(const std::string& command);
    bool validate_turn_command(const std::string& command);
};

#endif //CLIENT_CLIENT_H
