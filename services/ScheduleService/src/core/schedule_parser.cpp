#include "core/schedule_parser.h"
#include <iostream>

// парсим ответ от сервера
ScheduleData ScheduleParser::parseResponse(const std::string& jsonResponse){
    ScheduleData result;

    if (jsonResponse.empty()){
        std::cerr << "Error: Empty JSON response" << std::endl;
        return result;
    }

    try {
        auto jsonFile = nlohmann::json::parse(jsonResponse);
        if (jsonFile.is_array()){
            for (const auto& item : jsonFile){
                if (item.is_object()){
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
                    if (item.contains("dist")) {
                        try {
                            content.isDist = item["dist"].get<bool>();
                        } catch (...) {
                            content.isDist = item["dist"].get<int>() != 0;
                        }
                    } else {
                        content.isDist = false;
                    }

                    content.zoomLink = content.isDist ? item.value("zoom", "") : "";

                    if (item.contains("time") && item["time"].is_array() && item["time"].size() >= 2) {
                        content.time = {
                            item["time"][0].get<std::string>(),
                            item["time"][1].get<std::string>()
                        };
                    } else {
                        content.time = {"", ""};
                        std::cerr << "Warning: Invalid time format" << std::endl;
                    }
                    result.push_back(content);
                } else {
                    std::cerr << "Expected JSON object in array" << std::endl;
                }
            }
        } else {
            std::cerr << "Expected JSON array at top level" << std::endl;
        }
    } catch (const std::exception& e){
        std::cerr << "JSON parse error" << e.what() << std::endl;
        std::cerr << "Response content: " << jsonResponse << std::endl;
    } catch (const std::exception& e){
        std::cerr << "Error" << e.what() << std::endl;
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