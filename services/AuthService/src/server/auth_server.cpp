#include "server/auth_server.h"
#include "core/auth_parser.h"

bool AuthServer::startServer(int port){
    std::lock_guard<std::mutex> lock(server_mutex_);

    if (is_running_){
        return false;
    }    

    try {
        server_ = std::make_unique<httplib::Server>();

        server_->Get("/health", [](const httplib::Request&, httplib::Response& res){
            res.set_content("OK", "text/plain");
        });

        server_->Post("/api/auth", [this](const httplib::Request& req, httplib::Response& res){
            try {
                std::string login, password;

                if (!req.body.empty()){
                    auto json = nlohmann::json::parse(req.body);
                    login = json["login"].get<std::string>();
                    password = json["password"].get<std::string>();
                } else {
                    login = req.get_param_value("login");
                    password = req.get_param_value("password");
                }

                if (login.empty() || password.empty()) {
                    throw std::runtime_error("Login and password required");
                }

                std::string token = AuthParser::getToken(login, password);
                nlohmann::json response = {{"token", token}};
                res.set_content(response.dump(), "application/json");
            } catch (const std::exception& e){
                nlohmann::json error = {
                    {"error", "Bad Request"},
                    {"message", e.what()}
                };
                res.status = 400;
                res.set_content(error.dump(), "application/json");
            }
        });

        is_running_ = true;
        server_thread_ = std::thread([this, port](){
            try {
                std::cout << "Starting server on port " << port << std::endl;
                if (!server_->listen("0.0.0.0", port)){
                    std::cerr << "Failed to start server on port " << port << std::endl;
                    is_running_ = false;
                }
            } catch (const std::exception& e){
                std::cerr << "Server error: " << e.what() << std::endl;
                is_running_ = false;
            }
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return is_running_;
    } catch (const std::exception& e){
        std::cerr << "Server initialization failed: " << e.what() << std::endl;
        server_.reset();
        return false;
    }
}

void AuthServer::stopServer(){
    std::lock_guard<std::mutex> lock(server_mutex_);

    if (is_running_){
        is_running_ = false;
        if (server_){
            server_->stop();
        }
        if (server_thread_.joinable()) {
            server_thread_.join();
        }
        server_.reset();
    }
} 

bool AuthServer::isRunning() const {
    return is_running_;
}