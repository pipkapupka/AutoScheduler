#include "core/http_client.h"
#include <iostream>
#include "core/schedule_service.h"

// метод для записи ответа от сервера в буфер переменного размера
size_t HttpClient::writeCallback(void* contents, size_t size, size_t memb, std::string* output){
    size_t total_size = memb * size;
    output -> append((char*)contents, total_size);
    return total_size;
}

// выполнение запроса
std::string HttpClient::performRequest(const std::string& url){
    CURL* curl = curl_easy_init();
    std::string response;

    if (!curl) {
        throw std::runtime_error("Failed to initialize cURL");
    }

    std::cout << "Making request to: " << url << std::endl;
    std::cout << "Using API key: " << ScheduleService::apiKey << std::endl;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("apikey: " + ScheduleService::apiKey).c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK){
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return response;
}