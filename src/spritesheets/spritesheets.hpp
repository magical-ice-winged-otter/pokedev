#pragma once

#include <filesystem>

#include <SDL_image.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


extern const cv::Mat DEAD_IMAGE;

SDL_Texture* getSDLTexture(SDL_Renderer* renderer, cv::Mat m);

class SpriteSheetData {
    public:
        //vars
        const std::filesystem::path m_source {}; //if you need to track the multie spritesheet data, you can use this variable.
        //constructor
        SpriteSheetData(const std::filesystem::path& imageFile);
        //methods
        SDL_Texture* getSDLTexture(SDL_Renderer* renderer) {
            return ::getSDLTexture(renderer,  this->m_image);
        }

    private:
        cv::Mat m_image {};

};

