cmake_minimum_required(VERSION 3.16)

project(ScheduleService
    LANGUAGES CXX
    VERSION 1.0
    DESCRIPTION "Microservice for schedule management"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CURL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

include(FetchContent)
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib
    GIT_TAG master
)
FetchContent_MakeAvailable(httplib)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# компоненты для ядра
add_library(core
    src/core/schedule_service.cpp
    src/core/schedule_parser.cpp
    src/core/http_client.cpp
)
target_include_directories(core PUBLIC ${INCLUDE_DIR})
target_link_libraries(core PUBLIC ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)

# компоненты для билдера
add_library(builders
    src/builders/schedule_builder.cpp
)
target_include_directories(builders PUBLIC ${INCLUDE_DIR})
target_link_libraries(builders PUBLIC core)

# компоненты для сервера
add_library(servers
    src/servers/schedule_server.cpp
)
target_include_directories(servers PUBLIC ${INCLUDE_DIR})
target_link_libraries(servers PUBLIC builders httplib)

# главный исполняемый файл
add_executable(ScheduleService src/main.cpp)
target_link_libraries(ScheduleService PRIVATE servers core builders ${CURL_LIBRARIES} httplib)


