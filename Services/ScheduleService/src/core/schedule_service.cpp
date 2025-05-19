#include "core/schedule_service.h"
#include "builders/schedule_builder.h"

// костыль до тех пор пока не сделал переменные окружения
std::string ScheduleService::apiKey = "ddf4e7925ac8e9ebd0229797ce21c4d8";

void ScheduleService::setApiKey(const std::string& key){
    apiKey = key;
}