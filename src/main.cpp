#include "platform.hpp"
#include "application.hpp"

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
    printf("argc: %d", argc);
    if (argc == 2) {
        cv::Mat img = DrawUtil::loadImage(std::filesystem::path{"./tests/ow_char1.png"});
        /*
        std::function<std::optional<uchar*>(const DrawUtil::GeneratorPixel&)> callback = [](const DrawUtil::GeneratorPixel& genPixel) -> std::optional<uchar*> {
            for (int c = 0; c < genPixel.channels; ++c) {
                genPixel.pointer[c] = (genPixel.pointer[c] * 2) % 255 ;
            }

            return std::make_optional<uchar*>(genPixel.pointer);
        };
        
        DrawUtil::scanImage(img, callback);
        */
        int box[2][2] = {
            {0, 0}, 
            {32, 48}
        };
        int color[3] = {255, 255, 255};
        DrawUtil::scanSpriteBox(img, box, [img, color](const Rect& spriteBox) -> void {
            Scalar cv_color = Scalar(color[0], color[1], color[2]);
            rectangle(img, spriteBox.tl(), spriteBox.br(), cv_color, 1, 0);
        });
        imshow("Display window", img);
        waitKey(0);
        return 0;
    }
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