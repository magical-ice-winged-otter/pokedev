#include "spritesheets/draw.hpp"

Mat DrawUtil::loadImage(const std::filesystem::path& imageFile) {
    std::string input = imageFile.string();
    Mat img = imread(input, IMREAD_COLOR);
    if(img.empty()) {
        printf("Attempt to load image %s failed!", imageFile.string().c_str());
        return DrawUtil::DEAD_IMAGE;
    }
    return img;
}

void scanImage(Mat& mat, std::function<std::optional<uchar*>(DrawUtil::GeneratorPixel)> modify) {
    // accept only char type matrices
    CV_Assert(mat.depth() == CV_8U);
    int channels = mat.channels();
    int nRows = mat.rows;
    int nCols = mat.cols;
    if (mat.isContinuous()) {
        nCols *= nRows;
        nRows = 1;
    }
    uchar* p;
    for(int i = 0; i < nRows; ++i) {
        p = mat.ptr<uchar>(i);
        for (int j = 0; j < nCols; ++j) {
            uchar* col_ptr = &p[j];

            uchar copy[channels];
            for (int c = 0; c < channels; ++c) {
                copy[c] = col_ptr[c];
            }

            DrawUtil::GeneratorPixel pixel = { col_ptr, channels};
            std::optional<uchar*> modified = modify(pixel);
            if (!modified.has_value()) continue;
            for (int c = 0; c < channels; ++c) {
                col_ptr[c] = modified.value()[c];
            }
        }
    }
}

