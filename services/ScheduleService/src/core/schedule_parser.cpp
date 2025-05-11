#include "core/schedule_parser.h"
#include <iostream>

// парсим ответ от сервера
ScheduleData ScheduleParser::parseResponse(const std::string& jsonResponse){
    ScheduleData result;
    try {
        auto jsonFile = nlohmann::json::parse(jsonResponse);
        for (const auto& item : jsonFile){
            ScheduleItem content; // используем описанную ранее структуру с интересующими нас полями

            content.num = item.value("num", 0);
            content.typeLesson = item.value("typeLesson", "");
            content.dateLesson = item.value("date", "");
            content.group = item.value("group", "");
            content.lesson = item.value("lesson", "");
            content.teacher = item.value("teacher", "");
            content.cab = item.value("cab", "");

            // временное решение, пока зум есть в каждом json, нужно сделать чтобы он 
            // парсился только когда isDist == true
            if (item.contains("dist")){
                if (item["dist"].is_boolean()){
                    content.isDist = item["dist"].get<bool>();
                } else {
                    content.isDist = item["dist"].get<int>() != 0;
                }   
            } else {
                content.isDist = false;
            }

            content.zoomLink = item.value("zoom", "");

            if (item.contains("time")){
                content.time = {
                    item["time"].value("start", ""),
                    item["time"].value("end", "")
                };
            }
            result.push_back(content);
        }
    } catch (const std::exception& e){
        std::cerr << "JSON parse error" << e.what() << std::endl;
    }
    return result;
}

// метод для конвертации данных в json для передачи клиенту 
std::string ScheduleParser::scheduleDataToJson(const ScheduleData& data){
    nlohmann::json json;
    for (const auto& item : data){
        nlohmann::json itemJson;
        itemJson["num"] = item.num;
        itemJson["typeLesson"] = item.typeLesson;
        itemJson["dateLesson"] = item.dateLesson;
        itemJson["group"] = item.group;
        itemJson["teacher"] = item.teacher;
        itemJson["cab"] = item.cab;
        itemJson["zoomLink"] = item.zoomLink;
        itemJson["isDist"] = item.isDist;
        itemJson["time"] = {
            {"start", item.time.start},
            {"end", item.time.end}
        };
        json.push_back(itemJson);
    }
    // сереализация объекта в строку
    return json.dump();
}