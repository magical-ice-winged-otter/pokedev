#include "spritesheets/spritesheets.hpp"
#include "spritesheets/draw.hpp"
#include "spritesheets/palette_png.hpp"
#include <vector>
#include <map>

using namespace cv; //we are only going to use opencv namespace here for loading images ideally.

SpriteSheetData::SpriteSheetData(const std::filesystem::path& imageFile) : m_source{imageFile} {
    m_image = DrawUtil::loadImage(m_source);
};

Mat reOrderSheet(Mat& src, int box[2][2], std::vector<int> order);
SpriteSheetData::SpriteSheetData(const SpriteSheetData& data, int box[2][2], std::vector<int> order) {
    Mat old_image = data.m_image;
    m_image = reOrderSheet(old_image, box, order);
};

//https://stackoverflow.com/questions/22702630/converting-cvmat-to-sdl-texture
SDL_Texture* getSDLTexture(SDL_Renderer* renderer, Mat m_image) {
    SDL_Texture* tex = SDL_CreateTexture(renderer, 
        SDL_PIXELFORMAT_BGR24, 
        SDL_TEXTUREACCESS_STATIC, 
        m_image.cols,
        m_image.rows);
    SDL_UpdateTexture(tex, NULL, (void*) m_image.data, m_image.step1());

    return tex;
}

struct RectHash {
    std::size_t operator()(const Rect& rect) const {
        // by default, use the top left point
        Point point = rect.tl();
        return std::hash<long long>()(((long long) point.x) ^ (((long long) point.y) << 32));
    }
};

template<typename T>
constexpr ptrdiff_t index(std::vector<T> vector, T obj) {
    return std::distance(std::begin(vector), std::find(std::begin(vector), std::end(vector), obj));
}

Mat reOrderSheet(Mat& src, int box[2][2], std::vector<int> order) {
    std::vector<Rect> list;
    for (auto rect : DrawUtil::scanSpriteBox(src, box))  {
        list.push_back(rect);
    };
    while (order.size() < list.size()) {
        order.push_back(-1);
    } //this implies that order[a] is ignored if a > list.size
    size_t ignoreCount = std::count(order.begin(), order.end(), -1);
    size_t validSize = order.size() - ignoreCount;
    std::unordered_map<Rect, int, RectHash> order_map;
    for (int i = 0; i < list.size(); i++) {
        order_map[list[i]] = order[i];
    }
    std::sort(std::begin(list), std::end(list), [&order_map, &list, &order](const Rect& a, const Rect& b) -> bool {
        return order_map[a] < order_map[b];
    });

    Mat dest;
    //by default we make the assumptions that it's for pokeemerald type sheets
    //maybe later we make this more friendly
    //dest.create(src.rows, src.cols, src.type());
    BBox bbox = DrawUtil::fixBoundingBox(box);
    int width = (bbox[1][0] - bbox[0][0]) * std::min(list.size(), validSize);
    int height =  bbox[1][1] - bbox[0][1];
    dest.create(height, width, src.type());
    int i = ignoreCount;
    for (Rect rect : DrawUtil::scanSpriteBox(dest, box)) {
        Mat region {dest(rect)};
        Mat overwrite {src(list[i])};
        overwrite.copyTo(region);
        i++;  
    }
    std::cout << "done" << std::endl;
    //todo: need to fix the palette, but opencv doesn't provide that.
    return dest;
}

//Resizes the spritesheet by individually resizing each sprite and then collecting them back
Mat SpriteSheetData::resizeSheet(int box[2][2], int resizeBox[2][2], InterpolationFlags flag) {
    Mat src = this->m_image;
    int rowBoxes = src.rows / box[1][1];
    int colBoxes = src.cols / box[1][0];

    int newBoxWidth = resizeBox[1][0];
    int newBoxHeight = resizeBox[1][1];
    Mat dest;
    dest.create(rowBoxes * newBoxHeight, colBoxes * newBoxWidth, src.type());
    
    std::vector<Mat> destBox;
    
    for (auto& rect : DrawUtil::scanSpriteBox(src, box))  {
        Mat region {src(rect)};
        Mat resized;
        resize(region, resized, Size(newBoxWidth, newBoxHeight), 0, 0, flag);
        destBox.push_back(resized);
    }

    int i = 0;
    for (auto rect : DrawUtil::scanSpriteBox(dest, resizeBox))  {
        Mat region {dest(rect)};
        destBox[i].copyTo(region);
        i++;
    }

    return dest;
}