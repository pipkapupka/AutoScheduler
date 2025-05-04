cmake_minimum_required(VERSION 3.16)

include(FetchContent)
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib
    GIT_TAG master
)
FetchContent_MakeAvailable(httplib)

project(AuthService LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(AuthService main.cpp
    AuthService.h AuthService.cpp
)

target_link_libraries(AuthService
PRIVATE
    CURL::libcurl
    ZLIB::ZLIB
    nlohmann_json::nlohmann_json
    httplib
)

include(GNUInstallDirs)
install(TARGETS AuthService
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
