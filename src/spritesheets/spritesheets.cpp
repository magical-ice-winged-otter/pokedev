
#include "spritesheets.hpp"

using namespace cv; //we are only going to use opencv namespace here for loading images ideally.

const Mat DEAD_IMAGE(480, 480, CV_8UC3, Scalar(255, 255, 255));

SpriteSheetData::SpriteSheetData(const std::filesystem::path& imageFile) : m_source{imageFile} {
    m_image = loadImage(m_source);
};

//https://stackoverflow.com/questions/22702630/converting-cvmat-to-sdl-texture
SDL_Texture* getSDLTexture(SDL_Renderer* renderer, Mat m_image) {
    SDL_Texture* tex = SDL_CreateTexture(renderer, 
        SDL_PIXELFORMAT_BGR24, 
        SDL_TEXTUREACCESS_STATIC, 
        m_image.cols,
        m_image.rows);
    SDL_UpdateTexture(tex, NULL, (void*) m_image.data, m_image.step1());

    return tex;
}


namespace {
    Mat loadImage(const std::filesystem::path& imageFile) {
        Mat img = imread(imageFile.generic_string(), IMREAD_COLOR);
        if(img.empty()) {
            printf("Attempt to load image %s failed!", imageFile);
            return DEAD_IMAGE;
        }
        return img;
    }
}