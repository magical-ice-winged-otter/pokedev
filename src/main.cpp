#include <SDL.h>
#include "platform.hpp"
#include "application.hpp"

int main(int, char*[])
{
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