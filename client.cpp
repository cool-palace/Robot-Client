#include "client.h"

Client::Client(boost::asio::io_context& io_context, const std::string& server_ip, int port)
    : socket_(io_context), server_ip_(server_ip), server_port_(port) {}

void Client::start() {
    connect();
    listen_for_commands();
}

void Client::connect() {
    tcp::resolver resolver(socket_.get_executor());
    tcp::resolver::results_type endpoints = resolver.resolve(server_ip_, std::to_string(server_port_));
    boost::asio::connect(socket_, endpoints);
}

void Client::listen_for_commands() {
    std::string command;
    while (true) {
        std::cout << "Enter command (type 'pos' to get coordinates or 'exit' to quit): ";
        std::getline(std::cin, command);

        if (command == "exit") {
            std::cout << "Exiting...\n";
            break;
        } else if (command == "pos") {
            send_command(command);
        } else {
            std::cout << "Unknown command. Try again.\n";
        }
    }
}

void Client::send_command(const std::string& command) {
    boost::asio::write(socket_, boost::asio::buffer(command + "\n"));

    boost::asio::streambuf response_buffer;
    boost::asio::read_until(socket_, response_buffer, "\n");

    std::istream response_stream(&response_buffer);
    std::string response;
    std::getline(response_stream, response);
    std::cout << "Server reply: \nCoordinates: " << response << "\n";
}