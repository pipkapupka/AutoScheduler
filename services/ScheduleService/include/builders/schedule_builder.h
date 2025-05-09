#include "core/schedule_service.h"
#pragma once 

class ScheduleBuilder {
public:
    explicit ScheduleBuilder(ScheduleService& service); // инъекция зависимости через ссылку
    static ScheduleBuilder create(ScheduleService& service);

    ScheduleBuilder& setGroup(const std::string& group);
    ScheduleBuilder& setDate(const std::string& date);
    ScheduleBuilder& setTeacher(const std::string& teacher);
    ScheduleBuilder& setCab(const std::string& cab);
    ScheduleBuilder& zoomLink(bool isDist = true);
    
    ScheduleData execute();
private:
    ScheduleService& apiService_; // ссылка на сервис
    std::map<std::string, std::string> params_;
};