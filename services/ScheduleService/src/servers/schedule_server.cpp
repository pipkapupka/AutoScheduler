#include "servers/schedule_server.h"
#include "core/schedule_parser.h"

ScheduleServer::ScheduleServer(ScheduleService& service) : service_(service) {}

void ScheduleServer::startServer(int port){
    httplib::Server server;

    server.Get("/api/schedule", [this](const httplib::Request& req, httplib::Response& res){
        std::map<std::string, std::string> params;
        for (const auto& [key, value] : req.params){
            params[key] = value;
        }

        try {
            ScheduleData data = service_.getSchedule(params);
            res.set_content(ScheduleParser::scheduleDataToJson(data), "application/json");
        } catch (const std::exception& e){
            nlohmann::json error;
            error["error"] = e.what();
            res.set_content(error.dump(), "application/json");
        }
    });

    server.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("OK", "text/plain");
    });

    server.listen("0.0.0.0", port);
}