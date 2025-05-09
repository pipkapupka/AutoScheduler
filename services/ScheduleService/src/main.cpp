#include "servers/schedule_server.h"
#include "core/core_schedule_service.h"

int main() {
    CoreScheduleService core;
    ScheduleServer server(core);
    server.startServer(8080);
    return 0;
}