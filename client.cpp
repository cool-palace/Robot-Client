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
        } else if (command.substr(0,4) == "turn") {
            if (!validate_turn_command(command)) {
                continue;
            }
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

bool Client::validate_turn_command(const std::string& command) {
    std::istringstream iss(command);
    std::string turn_cmd, joint_arg, angle_arg;
    iss >> turn_cmd >> joint_arg >> angle_arg;

    if (turn_cmd != "turn" || joint_arg.substr(0, 8) != "--joint=" || angle_arg.substr(0, 8) != "--angle=") {
        std::cerr << "Invalid command format. Use: turn --joint=<number> --angle=<degrees>\n";
        return false;
    }

    int joint_number = std::stoi(joint_arg.substr(8));
    double angle = std::stod(angle_arg.substr(8));

    if (joint_number < 1 || joint_number > 6) {
        std::cerr << "Joint number must be between 1 and 6.\n";
        return false;
    }
    if (angle < -180 || angle > 180) {
        std::cerr << "Angle must be between -180 and 180 degrees.\n";
        return false;
    }
    return true;
}