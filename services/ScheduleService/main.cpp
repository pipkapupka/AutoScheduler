#include "scheduleservice.h"
#include <iostream>

int main() {
    ScheduleService::setApiKey("ddf4e7925ac8e9ebd0229797ce21c4d8");
    ScheduleService::startServer(8080);
    return 0;
}
