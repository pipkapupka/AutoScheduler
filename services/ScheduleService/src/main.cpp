#include "servers/schedule_server.h"
#include "builders/schedule_builder.h"
#include "core/core_schedule_service.h"
#include <csignal>

std::unique_ptr<ScheduleServer> global_server;

void signal_handler(int){
    if (global_server){
        global_server->stopServer();
    }
}

int main(int argc, char* argv[]) {
    int port = 8080;
    if (argc > 1) {
        port = std::atoi(argv[1]);
    }

    CoreScheduleService service;
    global_server = std::make_unique<ScheduleServer>(service);

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    
    if (!global_server->startServer(port)) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }

    std::cout << "Server is running on port " << port 
    << "\nPress Ctrl+C to stop..." << std::endl;

    while (global_server->isRunning()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}