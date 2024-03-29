#pragma "once"

#include <opencv2/opencv.hpp>


std::vector<uchar> imencode_palette_png(const cv::Mat &im_index, const cv::Mat &palette);
void imdecode_palette_png(const cv::Mat &bytes, cv::Mat &out_im_index, cv::Mat &out_palette);