#pragma once
#include "builders/schedule_builder.h"
#include <httplib.h>
#include <map>

class ScheduleServer {
public:
    explicit ScheduleServer(ScheduleService& service);
    void startServer(int port);
private:
    ScheduleService& service_;
};