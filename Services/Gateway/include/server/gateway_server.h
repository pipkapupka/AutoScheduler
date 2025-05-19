#include <httplib.h>
#include <memory>
#include <mutex>
#include <memory>
#include <atomic>

class GatewayServer {
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