cmake_minimum_required(VERSION 3.16)

project(ScheduleService
    LANGUAGES CXX
    VERSION 1.0
    DESCRIPTION "Microservice for schedule management"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib
    GIT_TAG master
)
FetchContent_MakeAvailable(httplib)

find_package(CURL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(SOURCES
    src/main.cpp
    src/core.cpp
    src/http_client.cpp
    src/schedule_builder.cpp
    src/schedule_parser.cpp
    src/schedule_server.cpp
    src/schedule_service.cpp
)

set(HEADERS
    include/core.h
    include/http_client.h
    include/schedule_builder.h
    include/schedule_parser.h
    include/schedule_server.h
    include/schedule_service.h
)

add_executable(ScheduleService ${SOURCES} ${HEADERS})

target_include_directories(ScheduleService
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CURL_INCLUDE_DIRS}
)

target_link_libraries(ScheduleService
PRIVATE
    CURL::libcurl
    ZLIB::ZLIB
    nlohmann_json::nlohmann_json
    httplib
)

include(GNUInstallDirs)

install(TARGETS ScheduleService
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
