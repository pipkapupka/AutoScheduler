#include "server/gateway_server.h"
#include <csignal>

std::unique_ptr<GatewayServer> global_server;

void signal_handler(int){
    if (global_server){
        global_server->stopServer();
    }
}

int main(int argc, char* argv[]){
    int port = 8082;
    if (argc > 1){
        port = std::stoi(argv[1]);
    }

    global_server = std::make_unique<GatewayServer>();

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    if (!global_server->startServer(port)){
        std::cerr << "Failed to start auth service" << std::endl;
        return 1;
    }

    std::cout << "Auth server is running on port " << port 
              << "\nPress Ctrl+C to stop..." << std::endl;
        
    while (global_server->isRunning()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
        
    return 0;
}