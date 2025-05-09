#pragma once
#include <map>
#include <string>
#include <nlohmann/json.hpp>

struct LessonTime {
    std::string start;
    std::string end;
};

struct ScheduleItem {
    size_t num;
    std::string typeLesson;
    std::string dateLesson;
    std::string group;
    std::string lesson;
    std::string teacher;
    std::string cab;
    std::string zoomLink;
    bool isDist;
    LessonTime time;
};

using ScheduleData = std::vector<ScheduleItem>;

class ScheduleService
{
public:
    static std::string apiKey;
    static void setApiKey(const std::string& key);
    virtual ScheduleData getSchedule(const std::map<std::string, std::string>& params) = 0;
    virtual ~ScheduleService() = default;
};

