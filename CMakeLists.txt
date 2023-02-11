cmake_minimum_required(VERSION 3.2)

project(MoonGB)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
    ${PROJECT_NAME}
    ./src/main.cpp
    ./src/utility/filedialogue.cpp
    ./src/emulator/gameboy.cpp
    ./src/emulator/cpu.cpp
    ./src/emulator/memory.cpp
    ./src/emulator/cartridge.cpp
    ./src/emulator/ppu.cpp
    ./src/program/config.cpp
    ./src/program/logger.cpp
    ./src/program/window.cpp
    ./src/program/program.cpp
    ./src/program/interface/gui_controller.cpp
    ./src/program/interface/gui_widget.cpp
    ./src/program/interface/gui_menu.cpp
    ./src/program/interface/gui_menu_controller.cpp
    ./src/program/interface/widgets/gui_label.cpp
    ./src/program/interface/widgets/gui_button.cpp
    ./src/program/interface/widgets/gui_box.cpp
    ./src/program/interface/menus/gui_test_menu.cpp
    ./src/program/interface/menus/gui_main_menu.cpp
    ./src/program/interface/menus/gui_motd_menu.cpp
    ./src/program/interface/menus/gui_now_playing_menu.cpp
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
    if(MSVC)
        target_compile_options(
            ${PROJECT_NAME} PUBLIC
            -Ox
        )
    else()
        target_compile_options(
            ${PROJECT_NAME} PUBLIC
            -Wall
            -Wpedantic
            -O3
        )
    endif()
else()
    if(MSVC)
        target_compile_options(
            ${PROJECT_NAME} PUBLIC
            -Od
            -DEBUG
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
endif()

target_compile_features(
    ${PROJECT_NAME} PUBLIC
    cxx_std_20
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_EXTENTSIONS OFF
)

