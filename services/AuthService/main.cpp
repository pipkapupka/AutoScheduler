#include "AuthService.h"
#include <thread>
#include <iostream>

int main(){
    std::thread([](){
        AuthService::startServer(8081);
    }).detach();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    try {
        auto token = AuthService::getToken("550032022000442", "4286964");
        std::cout << token << std::endl;
    } catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    }

    while (true){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
