#include "server/auth_server.h"
#include "core/auth_parser.h"
#include <csignal>

std::unique_ptr<AuthServer> global_server;

void signal_handler(int){
    if (global_server){
        global_server->stopServer();
    }
}

int main(int argc, char* argv[]){
    int port = 8081;
    if (argc > 1){
        port = std::stoi(argv[1]);
    }

    global_server = std::make_unique<AuthServer>();

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

