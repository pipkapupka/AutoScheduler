#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <httplib.h>
#pragma once

class AuthService
{
public:
    static std::string getToken(const std::string& login, const std::string& password);
    static void startServer(int port = 8081);
    static size_t writeCallback(void* contents, size_t size, size_t memb, std::string* output);
    static void setApiKey(const std::string& key);
private:
    static std::string apiKey;
};
