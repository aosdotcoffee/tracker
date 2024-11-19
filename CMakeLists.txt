cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# Define project
project(Tracker LANGUAGES C)
option(ASAN "Enable AddressSanitizer" OFF)
option(TSAN "Enable ThreadSanitizer" OFF)
option(UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(HARDENED "Enable various program hardening options" OFF)
option(BUILD_TESTS "Build test files" OFF)

# Third-party dependencies
add_subdirectory(Extern)

# Create the build_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add main target
add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -Wno-gnu-auto-type
    -Wno-gnu-zero-variadic-macro-arguments
    -Wno-fixed-enum-extension
    -std=c2x
)

add_compile_definitions(
    -Dauto=__auto_type
)

add_executable(Tracker "")
add_subdirectory(Source)

# Enable sanitizers
# Comes before the other libraries because ASAN needs to be first
if (ASAN OR HARDENED)
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

if (UBSAN OR HARDENED)
    add_compile_options(
        -fsanitize=undefined
    )

    target_link_libraries(Tracker
        PRIVATE
            ubsan
    )
endif()

if (HARDENED)
    add_compile_options(-fhardened)
endif()

target_link_libraries(Tracker
    PRIVATE
        Server
        Util
        tomlc99
)

target_include_directories(Tracker
    PRIVATE
        ${MMDB_INCLUDE_DIRS}
)

target_compile_options(Tracker
    PRIVATE
        ${MMDB_CFLAGS_OTHER}
)

# copy default config
if (NOT EXISTS ${CMAKE_BINARY_DIR}/config.toml)
    configure_file(${PROJECT_SOURCE_DIR}/Resources/config.toml ${CMAKE_BINARY_DIR}/config.toml COPYONLY)
endif()
