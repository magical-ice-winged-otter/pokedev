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
find_package(OpenCV CONFIG REQUIRED COMPONENTS core imgproc)
find_package(cppcoro CONFIG REQUIRED)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

# import cereal as a header-only library
file(GLOB CEREAL_HEADERS lib/include/cereal/**/*.hpp)
add_library(cereal INTERFACE)
target_include_directories(cereal INTERFACE lib/include)
target_sources(cereal INTERFACE ${CEREAL_HEADERS})


# all source files are defined inside `src/CMakeLists.txt`
add_subdirectory(src)
add_executable(pokedev ${SOURCE_FILES})

target_include_directories(pokedev PRIVATE include)

target_link_libraries(pokedev
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        imgui::imgui
        fmt::fmt
        opencv_core
        opencv_imgproc
        opencv_imgcodecs
        cereal
        cppcoro
)
