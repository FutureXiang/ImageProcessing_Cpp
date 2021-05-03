//
// Created by xiangweilai on 2021/4/27.
//

#ifndef IMAGE_IMAGE_H
#define IMAGE_IMAGE_H

#include <iostream>
#include <cassert>
#include <cmath>
#include "ByteReader.h"

#define GL_SILENCE_DEPRECATION

#include "GLUT/glut.h"
#include "GLUT/freeglut.h"

extern int argc_main;
extern char **argv_main;

double linearInterpolation(double A, double B, double ratio_Ax_AB);

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

    RGBdata getPixel(int row, int col);

    RGBdata getPixelFloat(double row, double col);

    void genBGR();

public:
    int height;
    int width;
    uint8_t *bgr1DAarray;

    explicit Image(const std::string &path);

    explicit Image(const Image *another);

    virtual ~Image();

    void show();

    void translation(int dx, int dy);

    void rotation(double theta);

    void scale(double cx, double cy);
};

#endif //IMAGE_IMAGE_H
