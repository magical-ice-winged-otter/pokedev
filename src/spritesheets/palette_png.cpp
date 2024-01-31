//CREDITS: https://gist.github.com/fzyzcjy/8b1736a294aab81eb86243bfb6fdc260
#include "spritesheets/palette_png.hpp"

using namespace std;
using namespace cv;

// NOTE: GOOD articles for reference
// 0. A blog with good tables and summary: https://rimson.top/2019/08/23/png-format/
// 1. The png spec about file structure: http://www.libpng.org/pub/png/spec/1.2/PNG-Structure.html

unsigned long crc(unsigned char *buf, int len);

const uchar PNG_SIGNATURE[] = {137, 80, 78, 71, 13, 10, 26, 10};

// NOTE png needs big endian https://stackoverflow.com/questions/2384111/png-file-format-endianness
uint32_t to_uint32_big_endian(const uchar *a) {
    return a[3] + (((uint32_t) a[2]) << 8) + (((uint32_t) a[1]) << 16) + (((uint32_t) a[0]) << 24);
}

void from_uint32_big_endian(uchar *a, uint32_t n) {
    a[0] = (n >> 24) & 0xFF;
    a[1] = (n >> 16) & 0xFF;
    a[2] = (n >> 8) & 0xFF;
    a[3] = n & 0xFF;
}

int png_iterator_first_chunk(const vector<uchar> &bytes) {
    size_t n = sizeof(PNG_SIGNATURE) / sizeof(PNG_SIGNATURE[0]);
    for (int j = 0; j < n; ++j) {
        CV_Assert(bytes[j] == PNG_SIGNATURE[j]);
    }
    return n;
}

int png_iterator_next_chunk(const vector<uchar> &bytes, int chunk_begin_loc, string &chunk_type) {
    int idx = chunk_begin_loc;

    uint32_t length = to_uint32_big_endian(&bytes[idx]);
    idx += 4;

    string type(bytes.begin() + idx, bytes.begin() + idx + 4);
    idx += 4;

    chunk_type = type;
    return chunk_begin_loc + (int) length + 12;
}

vector<uchar> create_PLTE_chunk(const Mat &palette) {
    CV_Assert(palette.cols == 1);
    CV_Assert(palette.type() == CV_8UC3);
    CV_Assert(palette.rows <= 256); // since only 8-bit depth!

    // *3: since 3 bytes per channel (color image)
    uint32_t length = palette.rows * 3;

    // +12: by definition of chunk "actual" length vs "recorded" length
    vector<uchar> bytes(length + 12);

    int idx = 0;

    from_uint32_big_endian(&bytes[idx], length);
    idx += 4;

    bytes[idx + 0] = 'P';
    bytes[idx + 1] = 'L';
    bytes[idx + 2] = 'T';
    bytes[idx + 3] = 'E';
    idx += 4;

    // I know not that fast, but palette is pretty small so readability is also important
    for (int j = 0; j < palette.rows; ++j) {
        for (int ch = 0; ch < 3; ++ch) {
            bytes[idx + ch] = palette.at<Vec3b>(j, 0)[ch];
        }
        idx += 3;
    }

    uint32_t crc_val = crc(&bytes[4], (int) length + 4);
    from_uint32_big_endian(&bytes[idx], crc_val);
    idx += 4;

    CV_Assert(idx == bytes.size());

    return bytes;
}

void change_IHDR_colortype_and_crc(vector<uchar> &bytes, int ihdr_start_loc, int ihdr_end_loc) {
    const int ihdr_data_loc = ihdr_start_loc + 4 + 4;
    const int ihdr_bitdepth_loc = ihdr_data_loc + 8;
    const int ihdr_colortype_loc = ihdr_data_loc + 9;
    const int crc_loc = ihdr_end_loc - 4;

    // we need 8-bit depth
    CV_Assert(bytes[ihdr_bitdepth_loc] == 8);

    // a. change colortype to "indexed color"
    bytes[ihdr_colortype_loc] = 3;

    // b. re-calculate the CRC value
    int chunk_length = ihdr_end_loc - ihdr_start_loc - 12;
    uint32_t crc_val = crc(&bytes[ihdr_start_loc + 4], chunk_length + 4);
    from_uint32_big_endian(&bytes[crc_loc], crc_val);
}

// just return vec by value, it is preferred https://stackoverflow.com/questions/15704565/efficient-way-to-return-a-stdvector-in-c
// NOTE WARN: 注意bmp那边返回的是Mat而非vector<uchar>，好像是vector有个什么性能问题(?)
vector<uchar> imencode_palette_png(const Mat &im_index, const Mat &palette) {
    CV_Assert(im_index.type() == CV_8UC1);
    CV_Assert(palette.cols == 1);
    CV_Assert(palette.type() == CV_8UC3);

    vector<uchar> bytes;
    imencode(".png", im_index, bytes,
            // use higher compression, thus smaller
             {IMWRITE_PNG_COMPRESSION, 9});

    int idx = png_iterator_first_chunk(bytes);
    while (true) {
        string chunk_type;
        int next_chunk_start_idx = png_iterator_next_chunk(bytes, idx, chunk_type);

        if (chunk_type == "IHDR") {

            // change 1: change THDR "color" flag
            change_IHDR_colortype_and_crc(bytes, idx, next_chunk_start_idx);

            // change 2: insert the PLTE chunk **after** THDR chunk
            vector<uchar> plte_chunk_bytes = create_PLTE_chunk(palette);
            bytes.insert(bytes.begin() + next_chunk_start_idx, plte_chunk_bytes.begin(), plte_chunk_bytes.end());

            // no need to manipulate the data after that. let's stop!
            break;
        }

        if (next_chunk_start_idx >= bytes.size()) break;

        idx = next_chunk_start_idx;
    }

    return bytes;
}

// ---------------------------------------------------------------------------------------------------------
// NOTE copy from: **OFFICIAL** png documentation has a link to "sample crc code"
// https://www.w3.org/TR/PNG-CRCAppendix.html
// ---------------------------------------------------------------------------------------------------------

/* Table of CRCs of all 8-bit messages. */
unsigned long crc_table[256];

/* Flag: has the table been computed? Initially false. */
int crc_table_computed = 0;

/* Make the table for a fast CRC. */
void make_crc_table(void) {
    unsigned long c;
    int n, k;

    for (n = 0; n < 256; n++) {
        c = (unsigned long) n;
        for (k = 0; k < 8; k++) {
            if (c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }
        crc_table[n] = c;
    }
    crc_table_computed = 1;
}

/* Update a running CRC with the bytes buf[0..len-1]--the CRC
   should be initialized to all 1's, and the transmitted value
   is the 1's complement of the final running CRC (see the
   crc() routine below)). */

unsigned long update_crc(unsigned long crc, unsigned char *buf, int len) {
    unsigned long c = crc;
    int n;

    if (!crc_table_computed)
        make_crc_table();
    for (n = 0; n < len; n++) {
        c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
    }
    return c;
}

/* Return the CRC of the bytes buf[0..len-1]. */
unsigned long crc(unsigned char *buf, int len) {
    return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
}

// ---------------------------------------------------------------------------------------------------------