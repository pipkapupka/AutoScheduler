cmake_minimum_required(VERSION 3.16)

project(AuthService
    LANGUAGES CXX
    VERSION 1.0
    DESCRIPTION "Microservice for auth"
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
    src/core/auth_parser.cpp
)
target_include_directories(core PUBLIC ${INCLUDE_DIR})
target_link_libraries(core PUBLIC ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)

# компоненты для сервера
add_library(server
    src/server/auth_server.cpp
)
target_include_directories(server PUBLIC ${INCLUDE_DIR})
target_link_libraries(server PUBLIC httplib)

# главный исполняемый файл
add_executable(AuthService src/main.cpp)
target_link_libraries(AuthService PRIVATE server core ${CURL_LIBRARIES} httplib)


