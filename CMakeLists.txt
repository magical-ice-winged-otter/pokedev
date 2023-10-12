cmake_minimum_required(VERSION 3.15)
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

project(pokedev
        VERSION 1.0
        DESCRIPTION "A set of tools for working with pokemon de-compilations."
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_WIN32_EXECUTABLE ON)

find_package(SDL2 CONFIG REQUIRED COMPONENTS SDL2 SDL2main)
find_package(SDL2_image CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

set(SOURCE_FILES
        "src/main.cpp"
        "src/porytiles_gui.cpp"
        "src/porytiles_command_generator.cpp"
        "src/imgui_utils.cpp"
        "src/platform/platform_windows.cpp"
        "src/platform.cpp"
        "src/serializer.cpp"
        "src/application.cpp"
)

set(INCLUDE_FILES
        "include/porytiles_gui.hpp"
        "include/porytiles_context.hpp"
        "include/porytiles_command_generator.hpp"
        "include/imgui_utils.hpp"
        "include/serializer.hpp"
        "include/platform.hpp"
        "include/platform/platform_windows.hpp"
        "include/application.hpp"
)

add_executable(pokedev ${SOURCE_FILES} ${INCLUDE_FILES})

target_include_directories(pokedev
        PRIVATE
        include
        lib/include
)

target_link_libraries(pokedev
        PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image
        imgui::imgui
)