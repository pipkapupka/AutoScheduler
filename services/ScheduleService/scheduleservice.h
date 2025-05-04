#ifndef SCHEDULESERVICE_H
#define SCHEDULESERVICE_H
#include <vector>
#include <map>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#pragma once

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
    static std::string scheduleDataToJson(const ScheduleData& data);
    static std::string handleGetSchedule(const std::map<std::string, std::string>& params);
    static void startServer(int port = 8080);
    static void setApiKey(const std::string& apikey);
    class Builder {
    public:
        Builder() = default;
        Builder& setGroup(const std::string& group);
        Builder& setDate(const std::string& date);
        Builder& setTeacher(const std::string& teacher);
        Builder& setCab(const std::string& cab);
        Builder& zoomLink(bool isDist = true);
        ScheduleData execute();
        static Builder create();
    private:
        std::map<std::string, std::string> params_;
    };
private:
    std::string performRequest(const std::string& url);
    ScheduleData parseResponse(const std::string& jsonResponse);
    static size_t writeCallback(void* contents, size_t size, size_t memb, std::string* output);
    static std::string apiKey;
};

#endif // SCHEDULESERVICE_H
