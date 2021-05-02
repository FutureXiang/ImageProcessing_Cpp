//
// Created by xiangweilai on 2021/4/27.
//

#ifndef IMAGE_IMAGE_H
#define IMAGE_IMAGE_H

#include <iostream>
#include <cassert>
#include "ByteReader.h"

#define GL_SILENCE_DEPRECATION

#include "GLUT/glut.h"

struct RGBdata {
    uint8_t R, G, B;

    RGBdata() = default;

    RGBdata(uint8_t r, uint8_t g, uint8_t b) {
        R = r;
        G = g;
        B = b;
    }

    friend std::ostream &operator<<(std::ostream &output, const RGBdata &p) {
        output << "R=" << (int) p.R << ", G=" << (int) p.G << ", B=" << (int) p.B;
        return output;
    }
};

class Image {
private:
    RGBdata **data;
public:
    int height;
    int width;
    uint8_t *bgr1DAarray;

    explicit Image(const std::string &path);

    RGBdata getPixel(int row, int col);

    void show();
};

#endif //IMAGE_IMAGE_H
