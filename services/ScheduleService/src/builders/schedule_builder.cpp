#include "builders/schedule_builder.h"
#include "core/schedule_service.h"
#include "core/http_client.h"
#include "core/schedule_parser.h"

// конструктор на основе Dependency Injection
ScheduleBuilder::ScheduleBuilder(ScheduleService& service) 
    : apiService_(service) {}

// статичный фабричный метод строителя
ScheduleBuilder ScheduleBuilder::create(ScheduleService& service){
    return ScheduleBuilder(service);
}

// методы fluent interface
ScheduleBuilder& ScheduleBuilder::setGroup(const std::string& group){
    params_["group"] = group;
    return *this;
}

ScheduleBuilder& ScheduleBuilder::setDate(const std::string& date){
    params_["date"] = date;
    return *this;
}

ScheduleBuilder& ScheduleBuilder::setTeacher(const std::string& teacher){
    params_["teacher"] = teacher;
    return *this;
}

ScheduleBuilder& ScheduleBuilder::setCab(const std::string& cab){
    params_["cab"] = cab;
    return *this;
}

ScheduleBuilder& ScheduleBuilder::zoomLink(bool isDist){
    params_["zoom"] = isDist ? "true" : "false";
    return *this;
}

// бизнес-логика выполнения запроса
ScheduleData ScheduleBuilder::execute(){
    std::string url = "https://api.ukrtb.ru/api/getSchedule?";
    CURL* curl = curl_easy_init();

    // кодируем параметры чтобы мозгана сайгасан не было
    for (const auto& param : params_){
        char* encoded_key = curl_easy_escape(curl, param.first.c_str(), 0);
        char* encoded_value = curl_easy_escape(curl, param.second.c_str(), 0);

        url += std::string(encoded_key) + "=" + std::string(encoded_value) + "&";

        curl_free(encoded_key);
        curl_free(encoded_value);
    }

    // удаляем последний символ & для корректного url
    if(!params_.empty()){
        url.pop_back();
    }

    std::string response;
    if (curl){
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpClient::writeCallback);
        curl_easy_setopt(curl, CURLOPT_HSTSWRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK){
            curl_easy_cleanup(curl);
            throw std::runtime_error("CURL error: " + std::string(curl_easy_strerror(res)));
        }

        curl_easy_cleanup(curl);
    }
    // возвращаем распарсенные данные через ScheduleService
    return ScheduleParser::parseResponse(response);
}