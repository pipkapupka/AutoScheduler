#include "servers/schedule_server.h"
#include "core/schedule_parser.h"
#include "core/schedule_service.h"
#include <chrono>

// Конструктор принимает ссылку на сервис, который будет обрабатывать запросы
ScheduleServer::ScheduleServer(ScheduleService& service) : service_(service) {}

bool ScheduleServer::startServer(int port){
    if (is_running_){
        return false;
    }

    auto server = std::make_shared<httplib::Server>(); //экземпляр hhtp сервера (пока не boost)

    // Эндпоинт для проверки здоровья сервера
    server->Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("OK", "text/plain");
    });

    // Основной эндпоинт для получения расписания
    server->Get("/api/schedule", [this](const httplib::Request& req, httplib::Response& res){
        std::map<std::string, std::string> params;
        for (const auto& [key, value] : req.params){
            params[key] = value;
        }

        try {
            ScheduleData data = service_.getSchedule(params);
            // Конвертируем данные в JSON и отправляем в клиент
            res.set_content(ScheduleParser::scheduleDataToJson(data), "application/json");
        } catch (const std::exception& e){
            nlohmann::json error;
            error["error"] = e.what();
            res.set_content(error.dump(), "application/json");
        }
    });

    // Устанавливаем флаг запуска сервера
    is_running_ = true;

    // Запускаем сервер в отдельном потоке, чтобы не блокировать основной поток от httplib
    server_thread_ = std::make_unique<std::thread>([server, port, this](){
        std::cout << "Starting server on port " << port << std::endl;
        if (!server->listen("0.0.0.0", port)) {
            std::cerr << "Failed to start server on port " << port << std::endl;
            is_running_ = false;
        }
    });

    // Даем время серверу запуститься
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return is_running_;
}

void ScheduleServer::stopServer(){
    if (is_running_){
        is_running_ = false;
        // Ожидаем завершения потока сервера
        if (server_thread_ && server_thread_->joinable()) {
            server_thread_->join();
        }
    }
}

bool ScheduleServer::isRunning() const {
    return is_running_;
}