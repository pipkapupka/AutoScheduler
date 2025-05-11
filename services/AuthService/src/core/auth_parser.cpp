#include "core/auth_parser.h"

std::string AuthParser::apiKey("ddf4e7925ac8e9ebd0229797ce21c4d8");

void AuthParser::setApiKey(const std::string& key){
    apiKey = key;
}

size_t AuthParser::writeCallback(void* contents, size_t size, size_t memb, std::string* output){
    size_t total_size = size * memb;
    output->append((char*)contents, total_size);
    return total_size;
}

std::string AuthParser::getToken(const std::string& login, const std::string& password){
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl){
        nlohmann::json request_data = {
            {"login", login},
            {"password", password}
        };
    
        std::string post_data = request_data.dump();

        std::string url = "https://api.ukrtb.ru/api/auth";

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("apikey: " + apiKey).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            throw std::runtime_error("Request failed");
        }

        try {
            auto json = nlohmann::json::parse(response);
            // если json с токеном вернулся - парсим его в строку, если нет выбрасываем исключения
            if (json.contains("token")) {
                return json["token"].get<std::string>();
            }
            throw std::runtime_error("Token not found in response");
        } catch(const std::exception& e) {
            throw std::runtime_error("JSON parse error: " + std::string(e.what()));
        }
    }
    // если с запросом не сложилось - выбрасываем исключение
    throw std::runtime_error("CURL init failed");
}
