#pragma once
#include "builders/schedule_builder.h"
#include <httplib.h>
#include <map>
#include <atomic>
#include <memory>

class ScheduleServer {
public:
    explicit ScheduleServer(ScheduleService& service);
    bool startServer(int port);
    void stopServer();
    bool isRunning() const;
private:
    // многопоточные приколы
    ScheduleService& service_;
    std::atomic<bool> is_running_{false};
    std::unique_ptr<std::thread> server_thread_;
};