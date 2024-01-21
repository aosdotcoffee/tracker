find_package(Git REQUIRED)

if(NOT EXISTS enet/CMakeLists.txt OR NOT EXISTS tomlc99/tomlc99/toml.h)
  execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive -- ${dir}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND_ERROR_IS_FATAL ANY)
endif()

add_subdirectory(enet6)
add_subdirectory(tomlc99)
