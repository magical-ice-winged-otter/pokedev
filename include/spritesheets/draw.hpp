#pragma once

#include <filesystem>
#include <optional>
#include <functional>
#include <coroutine>
#include <cppcoro/generator.hpp>
#include <vector>

#include <fmt/core.h>
#include <opencv2/opencv.hpp>


using namespace cv;
//https://docs.opencv.org/4.8.0/db/da5/tutorial_how_to_scan_images.html

//shorthand for a 2x2 int array
typedef std::array<std::array<int, 2>, 2> BBox;

namespace DrawUtil {
    const Mat DEAD_IMAGE(480, 480, CV_8UC3, Scalar(255, 255, 255));

    struct GeneratorPixel {
        uchar* pointer;
        int channels;
        int row;
        int col;
    };

    //this function will always modify the image
    //I decided that it's not useful to give a read only scan.
    cppcoro::generator<DrawUtil::GeneratorPixel> scanImage(Mat& mat);
    //Counts the colors in a mat
    //might be in BGR
    std::vector<std::vector<Scalar> > colors(Mat& mat);
    Mat loadImage(const std::filesystem::path& imageFile);
    inline void writeImage(const std::filesystem::path& imageFile, Mat& mat) {
        imwrite(imageFile.string(), mat);
    }
    
    BBox fixBoundingBox(int box[2][2]);
    cppcoro::generator<Rect> scanSpriteBox(Mat& mat, int box[2][2]);
}