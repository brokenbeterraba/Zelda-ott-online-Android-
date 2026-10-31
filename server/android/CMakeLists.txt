# Servidor ZeldaOnline embutido no app do jogo (um único APK).
# Biblioteca compartilhada separada, com símbolos ocultos: o cliente dentro de libsoh.so tem
# classes de mesmo nome (ZeldaOnline::ByteStream, sox, ...) e linkar estaticamente daria
# definições duplicadas.
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(ZO_SERVER_SRC "${CMAKE_CURRENT_LIST_DIR}/../src")

    file(GLOB_RECURSE ZO_SERVER_SOURCES CONFIGURE_DEPENDS "${ZO_SERVER_SRC}/*.cpp")
    list(FILTER ZO_SERVER_SOURCES EXCLUDE REGEX "/main\\.cpp$")

    file(READ "${CMAKE_CURRENT_LIST_DIR}/../config.json" ZO_DEFAULT_CONFIG)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/../config.json")
    configure_file("${CMAKE_CURRENT_LIST_DIR}/default_config.cpp.in"
                   "${CMAKE_CURRENT_BINARY_DIR}/default_config.cpp" @ONLY)

    add_library(ootserver SHARED
        ${ZO_SERVER_SOURCES}
        "${CMAKE_CURRENT_LIST_DIR}/ootserver_api.cpp"
        "${CMAKE_CURRENT_BINARY_DIR}/default_config.cpp")

    target_include_directories(ootserver
        PRIVATE "${ZO_SERVER_SRC}"
        PUBLIC  "${CMAKE_CURRENT_LIST_DIR}")
    target_compile_features(ootserver PRIVATE cxx_std_20)
    target_compile_options(ootserver PRIVATE -Wall -Wextra -Wno-unused-parameter -fvisibility=hidden -fvisibility-inlines-hidden)
    target_link_libraries(ootserver PRIVATE log)
endif()
