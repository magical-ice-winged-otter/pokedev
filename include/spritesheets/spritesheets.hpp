#pragma once

#include <filesystem>
#include <SDL_image.h>
#include <opencv2/opencv.hpp>

#include "spritesheets/draw.hpp"


extern const cv::Mat DEAD_IMAGE;

SDL_Texture* getSDLTexture(SDL_Renderer* renderer, cv::Mat m);

class SpriteSheetData {
    public:
        //vars
        const std::filesystem::path m_source {}; //if you need to track the multie spritesheet data, you can use this variable.
        cv::Mat m_image {};
        //constructor
        SpriteSheetData(const std::filesystem::path& imageFile);
        SpriteSheetData(const SpriteSheetData& data, int box[2][2], std::vector<int> order);
        //methods
        inline SDL_Texture* getSDLTexture(SDL_Renderer* renderer) {
            return ::getSDLTexture(renderer,  this->m_image);
        }
        Mat resizeSheet(int box[2][2], int resizeBox[2][2], InterpolationFlags flag);

    private:
        

};

