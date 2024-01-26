#pragma once

#include <filesystem>
#include <optional>
#include <functional>
#include <coroutine>
#include <cppcoro/generator.hpp>

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
    };

    //this function will always modify the image
    //I decided that it's not useful to give a read only scan.
    void scanImage(Mat& mat, std::function<std::optional<uchar*>(const DrawUtil::GeneratorPixel&)> modify);
    Mat loadImage(const std::filesystem::path& imageFile);
    inline void writeImage(const std::filesystem::path& imageFile, Mat& mat) {
        imwrite(imageFile.string(), mat);
    }
    
    BBox fixBoundingBox(int box[2][2]);
    cppcoro::generator<Rect> scanSpriteBox(Mat& mat, int box[2][2]);
}