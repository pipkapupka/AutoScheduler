#include "AuthService.h"
#include <thread>
#include <iostream>

int main(){
    // запускаем микросервис в новом потоке в фоне и делаем его независимым через detach()
    std::thread([](){
        AuthService::startServer(8081);
    }).detach();

    // даем время серверному потоку начать принимать соединения, так как основной поток уснул на секунду
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // тестируем получения токена
    try {
        auto token = AuthService::getToken("550032022000442", "4286964");
        std::cout << token << std::endl;
    } catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    }

    // пока костыльный бесконечный цикл ожидания, пока микросервис явно не остановят
    while (true){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
