cmake_minimum_required(VERSION 3.2)

project(MoonGB)

add_executable(
    ${PROJECT_NAME}
    ./src/main.cpp
)

find_package(
    SDL2 REQUIRED
)

find_package(
    SDL2_ttf REQUIRED
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
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

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/res/ ${CMAKE_CURRENT_BINARY_DIR}/res/
)
