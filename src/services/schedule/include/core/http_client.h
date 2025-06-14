#include <curl/curl.h>
#include <string>

class HttpClient {
public:
    static size_t writeCallback(void* contents, size_t size, size_t memb, std::string* output);
    static std::string performRequest(const std::string& url);
};