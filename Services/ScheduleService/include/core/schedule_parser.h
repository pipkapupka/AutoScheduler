#pragma once
#include <nlohmann/json.hpp>
#include "core/schedule_service.h"

class ScheduleParser {
public:
    static ScheduleData parseResponse(const std::string& jsonResponse);
    static std::string scheduleDataToJson(const ScheduleData& data); 
};