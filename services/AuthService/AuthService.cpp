#include "AuthService.h"
#include <iostream>

std::string AuthService::apiKey = "ddf4e7925ac8e9ebd0229797ce21c4d8";

void AuthService::setApiKey(const std::string& key) {
    apiKey = key;
}


std::string AuthService::getToken(const std::string& login, const std::string& password) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        nlohmann::json postAuthData;
        postAuthData ["login"] = login;
        postAuthData ["password"] = password;

        std::string postData = postAuthData.dump();

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

        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK || http_code != 200) {
            throw std::runtime_error("Request failed");
        }

        try {
            auto json = nlohmann::json::parse(response);
            if (json.contains("token")) {
                return json["token"].get<std::string>();
            }
            throw std::runtime_error("Token not found in response");
        } catch(const std::exception& e) {
            throw std::runtime_error("JSON parse error: " + std::string(e.what()));
        }
    }
    throw std::runtime_error("CURL init failed");
}

size_t AuthService::writeCallback(void* contents, size_t size, size_t memb, std::string * output){
    size_t total_size = size * memb;
    output->append((char*)contents, total_size);
    return total_size;
}

void AuthService::startServer(int port){
    httplib::Server server;

    server.Post("/api/auth", [](const httplib::Request& req, httplib::Response& res){
        try {
            std::string login;
            std::string password;

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

            std::cout << "Trying to auth " << login << std::endl;
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

    server.Get("/health", [](const httplib::Request&, httplib::Response& res){
        res.set_content("All good, brother", "text/plain");
    });

    std::cout << "Server shows signs of life on http:/localhost:" << port << std::endl;
    server.listen("0.0.0.0", port);
}
