#pragma once
#include <nlohmann/json.hpp>
#include <httplib.h>
#include <string>
#include <atomic>
#include <memory>
#include <mutex>

class AuthServer {
public:
    bool startServer(int port);
    void stopServer();
    bool isRunning() const;
private:
    mutable std::mutex server_mutex_;
    std::atomic<bool>is_running_{false};
    std::thread server_thread_;
    std::unique_ptr<httplib::Server> server_;
};