cmake_minimum_required(VERSION 3.16)
project(MicroCollege VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" OFF)

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/libs/cpp-dotenv/CMakeLists.txt")
	message(STATUS "Cloning cpp-dotend submodule..."
	execute_process(
		COMMAND git submodule update --init -- libs/cpp-dotenv
		WORKING_DIRECTORY ${CMAKE_SOUCE_DIR}
	)
endif()

add_subbdirectory(libs/cpp-dotenv)

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/services/ScheduleService/
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	cpp_dotenv
)

configure_file(
	${CMAKE_SOURCE_DIR}/.env
	${CMAKE_BINARY_DIR}/.env
	COPYONLY
)
