#include "spritesheets/spritesheets.hpp"
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

cppcoro::generator<DrawUtil::GeneratorPixel> DrawUtil::scanImage(Mat& mat) {
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
        for (int j = 0; j < nCols * channels; j = j + channels) {
            uchar* col_ptr = &p[j];

            DrawUtil::GeneratorPixel pixel = { col_ptr, channels, i, j };
            co_yield pixel;
            //todo: check if pixel is mutated
        }
    }
}

std::vector<std::vector<Scalar> > DrawUtil::colors(Mat& mat) {
    std::vector<std::vector<Scalar> > ret;
    for (DrawUtil::GeneratorPixel &pixel : DrawUtil::scanImage(mat)) {
        if (pixel.row == ret.size()) {
            ret.push_back({});
        }
        Scalar copy(pixel.pointer[0], pixel.pointer[1], pixel.pointer[2]);
        ret[pixel.row].push_back(copy);
    }

    return ret;
}

BBox DrawUtil::fixBoundingBox(int box[2][2]) {
    int upperLeftX = std::min(box[0][0], box[1][0]);
    int upperLeftY = std::min(box[0][1], box[1][1]);
    int bottomRightX = std::max(box[0][0], box[1][0]);
    int bottomRightY = std::max(box[0][1], box[1][1]);

    return {{{upperLeftX, upperLeftY}, {bottomRightX, bottomRightY}}};
}

//box[0] the offset (usually 0, 0)
//box[1] the actual box definition
cppcoro::generator<Rect> DrawUtil::scanSpriteBox(Mat& mat, int box[2][2]) {
    BBox fix = DrawUtil::fixBoundingBox(box);
    int upperLeftX = fix[0][0];
    int upperLeftY = fix[0][1];
    int bottomRightX = fix[1][0];
    int bottomRightY = fix[1][1];

    for (int i = upperLeftY; i < mat.rows; i = i + bottomRightY) {
        for (int j = upperLeftX; j < mat.cols; j = j + bottomRightX) {
            Rect spriteBox(j, i, bottomRightX, bottomRightY);
            std::cout << fmt::format("{} {}\n", spriteBox.tl().x, spriteBox.tl().y) << std::endl;
            co_yield spriteBox;
        }
    }
}