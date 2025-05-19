#pragma once 
#include <map>
#include <string>
#include <vector>

class ScheduleService;
struct ScheduleItem;
using ScheduleData = std::vector<ScheduleItem>;

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