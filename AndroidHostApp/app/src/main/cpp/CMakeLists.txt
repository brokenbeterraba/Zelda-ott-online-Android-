cmake_minimum_required(VERSION 3.22.1)
project(zelda_online_host LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# The host uses the exact server implementation shipped with this Shipwright-Online source.
get_filename_component(REPOSITORY_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../../../" ABSOLUTE)
set(SERVER_ROOT "${REPOSITORY_ROOT}/server/src")
if(NOT EXISTS "${SERVER_ROOT}/OOTServer.cpp")
    message(FATAL_ERROR "Shipwright-Online server source not found at ${SERVER_ROOT}")
endif()

file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS "${SERVER_ROOT}/*.cpp")
list(FILTER SERVER_SOURCES EXCLUDE REGEX "/main\\.cpp$")

add_library(ootserver_jni SHARED host_jni.cpp ${SERVER_SOURCES})
target_include_directories(ootserver_jni PRIVATE "${SERVER_ROOT}")
target_compile_options(ootserver_jni PRIVATE -Wall -Wextra -Wno-unused-parameter)
target_link_libraries(ootserver_jni PRIVATE log android)
