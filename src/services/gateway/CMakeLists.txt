cmake_minimum_required(VERSION 3.16)

project(Gateway
    LANGUAGES CXX
    VERSION 1.0
    PROJECT_DESCRIPTION "Microserves for routing"
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

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# компоненты для сервера
add_library(server
    src/server/gateway_server.cpp
)
target_include_directories(server PUBLIC ${INCLUDE_DIR})
target_link_libraries(server PUBLIC httplib)    

# компоненты для маршрутизатора
add_library(router
    src/router/gateway_router.cpp
)
add_executable(Gateway src/main.cpp)
target_link_libraries(Gateway PRIVATE server core ${CURL_LIBRARIES} httplib)