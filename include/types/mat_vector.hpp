#pragma once

#include "spritesheets/spritesheets.hpp"
#include "spritesheets/draw.hpp"

#include <vector>
#include <opencv2/opencv.hpp>


namespace MatCollection {
    struct Box {
        int width;
        int height;
    };

    class MatVector  {
        public:
            MatVector(const int &boxSizeX, const int &boxSizeY) : tileBox{boxSizeX, boxSizeY} {
                cv::Scalar defaultColor(12,12,255);
                STORAGE = cv::Mat(boxSizeX, boxSizeY, CV_8UC3, defaultColor);
            }
            MatVector() : MatVector(16, 16) {}
            ~MatVector() {
                //this->STORAGE.release();
            }

            bool add(Mat tile);
            bool remove(Mat tile);
            bool contains(Mat tile);

            std::size_t size();
            //How many boxX x boxY tiles can STORAGE fit.
            std::size_t spaceSize() {
                return (this->STORAGE.rows * this->STORAGE.cols) / (this->tileBox.width * this->tileBox.height);
            }

            inline SDL_Texture* getSDLTexture(SDL_Renderer* renderer) {
                return ::getSDLTexture(renderer,  this->STORAGE);
            }

        private: 
            cv::Mat STORAGE; //in here we will store the 2d tiles
            const Box tileBox;
            const std::map<int, int> slots; //if slot is in this set, that means that the area in the spot is taken by some box already. The order used to easily find the next slot to put in.
            //rvalue = 1 --> the spot is taken
            //rvalue = 0 --> the spot is not taken

            
            bool resize(int x, int y); //when we add a new box to the set, we may not have enough space, so we will we need to resize the Mat, copyMakeBorder
    };
}
