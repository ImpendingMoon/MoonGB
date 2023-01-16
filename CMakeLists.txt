cmake_minimum_required(VERSION 3.2)

project(MoonGB)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
    ${PROJECT_NAME}
    ./src/main.cpp
    ./src/program/config.cpp
    ./src/program/logger.cpp
    ./src/program/window.cpp
    ./src/program/program.cpp
)

find_package(
    SDL2 REQUIRED
)

find_package(
    fmt REQUIRED
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${SDL2_INCLUDE_DIRS}
    ${fmt_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    ${SDL2_LIBRARIES}
    fmt::fmt
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(
        ${PROJECT_NAME} PUBLIC
        -Wall
        -Wpedantic
        -O3
    )
else()
    target_compile_options(
        ${PROJECT_NAME} PUBLIC
        -Wall
        -Wpedantic
        -O0
        -g
    )
endif()

target_compile_features(
    ${PROJECT_NAME} PUBLIC
    cxx_std_20
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_EXTENTSIONS OFF
)

