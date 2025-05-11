#pragma once

#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <string>

class AuthParser {
public:
    static std::string getToken(const std::string& login, const std::string& password);
    static size_t writeCallback(void* contents, size_t size, size_t memb, std::string* output);
    static void setApiKey(const std::string& key);
private:
    static std::string apiKey;
};