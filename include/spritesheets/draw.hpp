#pragma once

#include <filesystem>
#include <optional>
#include <functional>

#include <opencv2/opencv.hpp>


using namespace cv;
//https://docs.opencv.org/4.8.0/db/da5/tutorial_how_to_scan_images.html


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
    void scanSpriteBox(Mat& mat, int box[2][2], std::function<void(const Rect&)> boxConsumer);
}