#include "AuthService.h"
#include <iostream>

// 2 костыля которые находятся здесь до поры до времени, пока не реализую .env / параметры окружения
std::string AuthService::apiKey = "ddf4e7925ac8e9ebd0229797ce21c4d8";

void AuthService::setApiKey(const std::string& key) {
    apiKey = key;
}

// Метод для получения токена, путём отправки запроса к API сервиса и парсингом токена на выхода
std::string AuthService::getToken(const std::string& login, const std::string& password) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        nlohmann::json postAuthData;
        postAuthData ["login"] = login;
        postAuthData ["password"] = password;

        std::string postData = postAuthData.dump();

        // сервис к которому пока обращается микросервис
        std::string url = "https://api.ukrtb.ru/api/auth";

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("apikey: " + apiKey).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK || http_code != 200) {
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

// Стандартная запись ответа от сервера в буфер переменного размера (если место в буфере заканчивается - выделяется буфер с размером в 2 раза больше)
size_t AuthService::writeCallback(void* contents, size_t size, size_t memb, std::string * output){
    size_t total_size = size * memb;
    output->append((char*)contents, total_size);
    return total_size;
}

// Запускаем сервер
void AuthService::startServer(int port){
    httplib::Server server;

    // Реализация POST запроса на получения bearer-токена
    server.Post("/api/auth", [](const httplib::Request& req, httplib::Response& res){
        try {
            std::string login;
            std::string password;

            // Ряд проверок чтобы все было в шоколаде
            if (!req.body.empty()){
                auto json = nlohmann::json::parse(req.body);
                login = json["login"];
                password = json["password"];
            } else {
                login = req.get_param_value("login");
                password = req.get_param_value("password");
            }

            if (login.empty() || password.empty()){
                throw std::runtime_error("Login and password required");
            }

            // Записываем токен в переменную и возвращаем
            std::string token = AuthService::getToken(login, password);

            nlohmann::json response;
            response["token"] = token;
            res.set_content(response.dump(), "application/json");

        } catch(const std::exception& e){
            nlohmann::json error;
            error["error"] = "Bad request";
            error["message"] = e.what();
            res.status = 400;
            res.set_content(error.dump(), "application/json");
            std::cerr << "Error: " << e.what() << std::endl;
        }
    });

    // Проверяем жив ли сервер
    server.Get("/health", [](const httplib::Request&, httplib::Response& res){
        res.set_content("All good, brother", "text/plain");
    });

    std::cout << "Server shows signs of life on http:/localhost:" << port << std::endl;
    server.listen("0.0.0.0", port);
}
