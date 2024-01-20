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
if (SANITIZERS)
    add_compile_options(
        -fsanitize=address
        -fsanitize=undefined
    )

    target_link_libraries(Tracker
        PRIVATE
            asan
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
)
