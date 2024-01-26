#include "platform.hpp"
#include "application.hpp"

#include <fmt/core.h>
#include <filesystem>
#include "spritesheets/draw.hpp"
#include "spritesheets/spritesheets.hpp"


/*
std::optional<uchar*> callback(DrawUtil::GeneratorPixel& genPixel) {
    printf("channels: %d", genPixel.channels);
    for (int c = 0; c < genPixel.channels; ++c) {
        genPixel.pointer[c] = (genPixel.pointer[c] * 2) % 255 ;
    }

    return std::make_optional<uchar*>(genPixel.pointer);
}
*/

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