//
// Created by xiangweilai on 2021/4/27.
//

#ifndef IMAGE_IMAGE_H
#define IMAGE_IMAGE_H

#include <iostream>
#include "ByteReader.h"

#define GL_SILENCE_DEPRECATION

#include "GLUT/glut.h"

struct RGB {
    uint8_t R, G, B;

    RGB() = default;

    RGB(uint8_t r, uint8_t g, uint8_t b) {
        R = r;
        G = g;
        B = b;
    }

    friend std::ostream &operator<<(std::ostream &output, const RGB &p) {
        output << "R=" << (int) p.R << ", G=" << (int) p.G << ", B=" << (int) p.B;
        return output;
    }
};

class Image {
private:
    RGB **data;
public:
    int height;
    int width;
    uint8_t *bgr1DAarray;

    explicit Image(const std::string &path);

    RGB getPixel(int row, int col);

    void show();
};

#endif //IMAGE_IMAGE_H
