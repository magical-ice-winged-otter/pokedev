# todo: would love to simplify this file, but im honestly not good enough w/ cmake to do it well

cmake_minimum_required(VERSION 3.15)
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

project(pokedev
        VERSION 1.0
        DESCRIPTION "A set of tools for working with pokemon de-compilations."
        LANGUAGES CXX
)

find_package(SDL2 CONFIG REQUIRED COMPONENTS SDL2 SDL2main)
find_package(SDL2_image CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# stops the console from popping up, probably windows-only
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_FILES
        src/main.cpp
        src/application.hpp
        src/application.cpp
        src/imgui_utils.cpp
        src/imgui_utils.hpp
        src/serializer.cpp
        src/serializer.hpp
        src/text_parser.cpp
        src/text_parser.hpp
        src/game_loaders.hpp
        src/game_loaders.cpp
        src/platform.cpp
        src/platform.hpp
        src/platform/platform_windows.cpp
        src/platform/platform_windows.hpp
        src/porytiles/porytiles_gui.cpp
        src/porytiles/porytiles_gui.hpp
        src/porytiles/porytiles_context.hpp
        src/porytiles/porytiles_command_generator.cpp
        src/porytiles/porytiles_command_generator.hpp
        src/trainers/trainer_editor.cpp
        src/trainers/trainer_editor.hpp
        src/trainers/mons/mon_editor.hpp
        src/trainers/mons/mon_editor.cpp
        src/trainers/mons/mon_data.hpp
        src/game_settings.hpp
        src/game_settings.cpp
        src/trainers/mons/mon_data.cpp
        src/shortcuts/shortcuts.hpp
        src/shortcuts/shortcuts.cpp
        src/string_parsing_util.hpp
        src/string_parsing_util.cpp
)

add_executable(pokedev ${SOURCE_FILES})

target_include_directories(pokedev
        PRIVATE
        src
        lib/include
)

target_link_libraries(pokedev
        PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image
        imgui::imgui
)
