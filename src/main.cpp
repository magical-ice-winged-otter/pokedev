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
    if (argc == 2) {
        
        SpriteSheetData data(std::filesystem::path{argv[1]});
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
        //std::vector<int> order = {1, 4, 10, 5, 3, 8, 16, 9, 11, 12, 13, 14,  2,  6, 15,  7};
        std::vector<int> order = {1, 4, -1, 5, 3, 8, -1, 9, -1, -1, -1, -1,  2,  6, -1,  7};
        
        /*
        int color[3] = {255, 255, 255};
        DrawUtil::scanSpriteBox(img, box, [img, color](const Rect& spriteBox) -> void {
            Scalar cv_color = Scalar(color[0], color[1], color[2]);
            rectangle(img, spriteBox.tl(), spriteBox.br(), cv_color, 1, 0);
        });
        */     
        SpriteSheetData reorder(data, box, order);
        imshow("Another window", data.m_image);
        imshow("Display window", reorder.m_image);

        imwrite("./tests/output.png", reorder.m_image);
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