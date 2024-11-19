# Dependencies
find_package(Threads REQUIRED)
find_package(PkgConfig)
pkg_check_modules(MMDB REQUIRED libmaxminddb)

target_sources(Tracker
    PRIVATE
        Main.c
)

target_compile_features(Tracker
    PRIVATE
        c_std_23
)

add_library(TrackerCommon INTERFACE)

target_include_directories(TrackerCommon
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/Extern
        ${PROJECT_SOURCE_DIR}/Extern/tomlc99
        ${PROJECT_SOURCE_DIR}/Extern/enet6/include
)

add_subdirectory(Util)
add_subdirectory(Server)
if (BUILD_TESTS)
    include(CTest)
    add_subdirectory(Tests)
endif()

target_link_libraries(Server
    PRIVATE
        TrackerCommon
        Util
)
