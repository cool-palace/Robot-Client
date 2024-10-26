#include "client.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <server_port>\n";
        return 1;
    }
    try {
        int port = std::stoi(argv[1]);
        boost::asio::io_context io_context;
        Client client(io_context, "127.0.0.1", port);
        client.start();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}