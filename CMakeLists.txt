cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# Define project
project(Tracker)

# Dependencies
find_package(Threads REQUIRED)

# Third-party dependencies
add_subdirectory(Extern)

# Create the build_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add main target
add_compile_options(
    -Wall
    -Wextra
    -Werror
    -Wpedantic
    -std=gnu11
    -fstack-protector-strong
)

add_executable(Tracker "")
add_subdirectory(Source)

# Enable sanitizers
# Comes before the other libraries because ASAN needs to be first
if (ASAN)
    if (TSAN)
        message(FATAL_ERROR "You cannot enable ASAN and TSAN at the same time")
    endif()

    add_compile_options(
        -fsanitize=address
    )

    target_link_libraries(Tracker
        PRIVATE
            asan
    )
endif()

if (TSAN)
    add_compile_options(
        -fsanitize=thread
    )

    target_link_libraries(Tracker
        PRIVATE
            tsan
    )
endif()

if (UBSAN)
    add_compile_options(
        -fsanitize=undefined
    )

    target_link_libraries(Tracker
        PRIVATE
            ubsan
    )
endif()

target_link_libraries(Tracker
    PRIVATE
        Server
        Util
        enet
        m
        json-c
        Threads::Threads
        tomlc99
        microhttpd
)

# copy default config
if (NOT EXISTS ${CMAKE_BINARY_DIR}/config.toml)
    configure_file(${PROJECT_SOURCE_DIR}/Resources/config.toml ${CMAKE_BINARY_DIR}/config.toml COPYONLY)
endif()
