#include "platform.hpp"
#include "application.hpp"

#include <fmt/core.h>
#include <filesystem>

int main(int argc, char* argv[]) {
    Platform::init();
    Application::init();
        
    while (!Platform::wantsToQuit())
    {
        Platform::startFrame();
        Application::render();
        Platform::endFrame();
    }

    Application::shutdown();
    Platform::shutdown();   
    return 0;
}