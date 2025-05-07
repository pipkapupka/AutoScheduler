#include "scheduleservice.h"
#include <curl/curl.h>
#include <httplib.h>
#include <iostream>
#include <locale>
#include <sstream>

// костыли для тестирования кода, позже вынесу в параметры окружения
std::string ScheduleService::apiKey = "ddf4e7925ac8e9ebd0229797ce21c4d8";

void ScheduleService::setApiKey(const std::string& key) {
    apiKey = key;
}

// паттерн проектирования "строитель" для строительства составного объекта, а именно тела запроса к API сервису
// далее идут вариации конкретных строителей
ScheduleService::Builder& ScheduleService::Builder::setGroup(const std::string& group) {
    params_["group"] = group;
    return *this;
}

ScheduleService::Builder& ScheduleService::Builder::setCab(const std::string& cab) {
    params_["cab"] = cab;
    return *this;
}

ScheduleService::Builder& ScheduleService::Builder::setDate(const std::string& date) {
    params_["date"] = date;
    return *this;
}

ScheduleService::Builder& ScheduleService::Builder::setTeacher(const std::string& teacher) {
    params_["teacher"] = teacher;
    return *this;
}

ScheduleService::Builder& ScheduleService::Builder::zoomLink(bool isDist) {
    params_["zoom"] = isDist ? "true" : "false";
    return *this;
}

// Стандартная запись ответа от сервера в буфер переменного размера (если место в буфере заканчивается - выделяется буфер с размером в 2 раза больше)
size_t ScheduleService::writeCallback(void* contents, size_t size, size_t memb, std::string* output) {
    size_t total_size = size * memb;
    output->append((char*)contents, total_size);
    return total_size;
}

// Отдельный метод для отправки запроса к сервису
std::string ScheduleService::performRequest(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("apikey: " + apiKey).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return response;
}

// Парсинг полученного json'а для дальнейшего использования на клиенте
ScheduleData ScheduleService::parseResponse(const std::string& jsonResponse) {
    ScheduleData result;
    try {
        auto jsonFile = nlohmann::json::parse(jsonResponse);
        for (const auto& item : jsonFile) {
            ScheduleItem content;
            content.num = item.value("num", 0);
            content.typeLesson = item.value("typeLesson", "");
            content.dateLesson = item.value("date", "");
            content.group = item.value("group", "");
            content.lesson = item.value("lesson", "");
            content.teacher = item.value("teacher", "");
            content.cab = item.value("cab", "");

            // проверка на дистанционность пары + из-за особенности API сервиса добавлена конвертация из 0 и 1 в t/f
            if (item.contains("dist")) {
                if (item["dist"].is_boolean()) {
                    content.isDist = item["dist"].get<bool>();
                }
                else {
                    content.isDist = item["dist"].get<int>() != 0;
                }
            }
            else {
                content.isDist = false;
            }

            // to-do: переделать что если пара дистанционная - парсится ссылка на zoom-конференцию
            content.zoomLink = item.value("zoom", "");

            if (item.contains("time")) {
                content.time = {
                    item["time"].value("start", ""),
                    item["time"].value("end", "")
                };
            }
            result.push_back(content);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    }
    return result;
}

// "Продукт" строителя + отправка запроса при помощи вышеописанных отдельных функций
ScheduleData ScheduleService::Builder::execute() {
    ScheduleService Api;
    std::string url = "https://api.ukrtb.ru/api/getSchedule?";

    CURL* curl = curl_easy_init();
    for (const auto& param : params_) {
        char* encoded_key = curl_easy_escape(curl, param.first.c_str(), 0);
        char* encoded_value = curl_easy_escape(curl, param.second.c_str(), 0);

        url += std::string(encoded_key) + "=" + std::string(encoded_value) + "&";

        curl_free(encoded_key);
        curl_free(encoded_value);
    }

    if (curl) curl_easy_cleanup(curl);

    if (!params_.empty()) {
        url.pop_back();
    }

    std::cout << "RequestURL " << url << std::endl;
    std::string response = Api.performRequest(url);
    std::cout << "Raw API Response: " << response << std::endl;
    return Api.parseResponse(response);
}

// парсинг полученных данных в дальнейшее расписание с которым будем работать
std::string ScheduleService::scheduleDataToJson(const ScheduleData& data) {
    nlohmann::json json;
    for (const auto& item : data) {
        nlohmann::json itemJson;
        itemJson["num"] = item.num;
        itemJson["typeLesson"] = item.typeLesson;
        itemJson["dateLesson"] = item.dateLesson;
        itemJson["group"] = item.group;
        itemJson["lesson"] = item.lesson;
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
    return json.dump();
}

std::string ScheduleService::handleGetSchedule(const std::map<std::string, std::string>& params) {
    Builder builder;
    for (const auto& [key, value] : params) {
        if (key == "group") builder.setGroup(value);
        else if (key == "date") builder.setDate(value);
        else if (key == "teacher") builder.setTeacher(value);
        else if (key == "cab") builder.setCab(value);
        else if (key == "zoom") builder.zoomLink(value == "true");
    }

    try {
        ScheduleData data = builder.execute();
        return scheduleDataToJson(data);
    }
    catch (const std::exception& e) {
        nlohmann::json error;
        error["Something went wrong..."] = e.what();
        return error.dump();
    }
}

// Метод для запуска сервера
void ScheduleService::startServer(int port) {
    httplib::Server server;

    server.Get("/api/schedule", [](const httplib::Request& req, httplib::Response& res) {
        std::map<std::string, std::string> params;
        for (const auto& [key, value] : req.params) {
            params[key] = value;
        }

        std::string result = handleGetSchedule(params);
        res.set_content(result, "application/json");
    });

    // Проверяем что сервер жив
    server.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("OK", "text/plain");
    });

    std::cout << "Server is working on http:/localhost:" << port << std::endl;
    server.listen("0.0.0.0", port);
}

// "Директор" строителя
ScheduleService::Builder ScheduleService::Builder::create() {
    return ScheduleService::Builder();
}
