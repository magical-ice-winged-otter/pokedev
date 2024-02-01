#ifndef POKEDEV_PALETTE_PNG_HPP
#define POKEDEV_PALETTE_PNG_HPP

#include <opencv2/opencv.hpp>


std::vector<uchar> imencode_palette_png(const cv::Mat &im_index, const cv::Mat &palette);
void imdecode_palette_png(const cv::Mat &bytes, cv::Mat &out_im_index, cv::Mat &out_palette);

#endif // POKEDEV_PALETTE_PNG_HPP