cmake_minimum_required(VERSION 3.15)
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

project(PorytilesGui
        VERSION 1.0
        DESCRIPTION "A cross-platform GUI wrapper for porytiles."
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 CONFIG REQUIRED COMPONENTS SDL2 SDL2main)
find_package(imgui CONFIG REQUIRED)
find_package(unofficial-nativefiledialog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

set(SOURCE_FILES
        "src/main.cpp"
        "src/porytiles_gui.cpp"
        "src/file_dialog.cpp"
        "src/serializer.cpp"
        "src/sdl_imgui_wrapper.cpp"
        "src/porytiles_command_generator.cpp"
        "src/imgui_utils.cpp"
)

set(INCLUDE_FILES
        "include/file_dialog.hpp"
        "include/serializer.hpp"
        "include/sdl_imgui_wrapper.hpp"
        "include/porytiles_gui.hpp"
        "include/porytiles_context.hpp"
        "include/porytiles_command_generator.hpp"
        "include/imgui_utils.hpp"
)

add_executable(porytiles_gui ${SOURCE_FILES} ${INCLUDE_FILES})

target_include_directories(porytiles_gui
        PRIVATE
        include
)

target_link_libraries(porytiles_gui
        PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
        imgui::imgui
        unofficial::nativefiledialog::nfd
        nlohmann_json::nlohmann_json
        fmt::fmt
        SDL2_image::SDL2_image
)