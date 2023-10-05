cmake_minimum_required(VERSION 3.15)
set(CMAKE_TOOLCHAIN_FILE ./vcpkg/scripts/buildsystems/vcpkg.cmake)

project(PORYTILES-GUI
        VERSION 1.0
        DESCRIPTION "A cross-platform GUI wrapper for porytiles."
        LANGUAGES CXX
)

find_package(SDL2 CONFIG REQUIRED COMPONENTS SDL2 SDL2main)
find_package(imgui CONFIG REQUIRED)

set(SOURCE_FILES 
        "src/main.cpp"
)

add_executable(porytiles_gui ${SOURCE_FILES})

target_link_libraries(porytiles_gui
        PRIVATE
                SDL2::SDL2
                SDL2::SDL2main
                imgui::imgui
)