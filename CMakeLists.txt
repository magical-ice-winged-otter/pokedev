# todo: would love to simplify this file, but im honestly not good enough w/ cmake to do it well

cmake_minimum_required(VERSION 3.20...3.28)
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

project(pokedev
        VERSION 1.0
        DESCRIPTION "A set of tools for working with pokemon de-compilations."
        LANGUAGES CXX
)

add_compile_options("-fcoroutines")

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

set(SOURCE_FILES
        src/tools/imgui_demo_tool.cpp
        src/main.cpp
        src/application.cpp
        src/imgui_utils.cpp
        src/serializer.cpp
        src/text_parser.cpp
        src/game_loaders.cpp
        src/platform.cpp
        src/platform/platform_windows.cpp
        src/porytiles/porytiles_gui.cpp
        src/porytiles/porytiles_command_generator.cpp
        src/trainers/trainer_editor.cpp
        src/trainers/mons/mon_editor.cpp
        src/game_settings.cpp
        src/trainers/mons/mon_data.cpp
        src/tools/shortcuts_tool.cpp
        src/string_parsing_util.cpp
)

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(pokedev ${SOURCE_FILES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_include_directories(pokedev
        PRIVATE
        include
        lib/include
)

target_link_libraries(pokedev
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        imgui::imgui
        fmt::fmt
        ${OpenCV_LIBS}
)
