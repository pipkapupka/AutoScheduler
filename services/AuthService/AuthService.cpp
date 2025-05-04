#include "AuthService.h"
#include <iostream>

std::string AuthService::apiKey = "";

void AuthService::setApiKey(const std::string& key) {
    apiKey = key;
}

std::string AuthService::getToken(const std::string& login, const std::string& password){
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl){
        char* request_login = curl_easy_escape(curl, login.c_str(), login.length());
        char* request_password = curl_easy_escape(curl, password.c_str(), password.length());
        std::string url = std::string("https://api.ukrtb.ru/api/auth?login=") + request_login + "&password=" + request_password;

        curl_free(request_login);
        curl_free(request_password);
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("apikey: " + apiKey).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK){
            throw std::runtime_error("Something went wrong");
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
        
    try {
        auto json = nlohmann::json::parse(response);
        return json["token"].get<std::string>();
    } catch(const std::exception& e){
        throw std::runtime_error("Failed to parse token" + std::string(e.what()));
    }
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
