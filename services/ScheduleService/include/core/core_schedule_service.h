#pragma once
#include "core/schedule_service.h"
#include "builders/schedule_builder.h"

class CoreScheduleService : public ScheduleService {
public:
    ScheduleData getSchedule(const std::map<std::string, std::string>& params) override {
        ScheduleBuilder builder(*this);

        for (const auto& [key, value] : params){
            if (key == "group") builder.setGroup(value);
            else if (key == "date") builder.setDate(value);
            else if (key == "teacher") builder.setTeacher(value);
            else if (key == "cab") builder.setCab(value);
            else if (key == "zoom") builder.zoomLink(value == "true");
        }
        
        return builder.execute();
    }
};