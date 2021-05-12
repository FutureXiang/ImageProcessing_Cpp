//
// Created by xiangweilai on 2021/4/27.
//

#ifndef IMAGE_IMAGE_H
#define IMAGE_IMAGE_H

#include "Color.h"
#include "ByteReader.h"
#include "Mathematics.h"

#define GL_SILENCE_DEPRECATION

#include "GLUT/glut.h"
#include "GLUT/freeglut.h"

extern int argc_main;
extern char **argv_main;

double linearInterpolation(double A, double B, double ratio_Ax_AB);

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

    void show(const char *title);

    void translation(int dx, int dy);

    void rotation(double theta);

    void scale(double cx, double cy);

    void equalizeHist();

    void showFourier();
};

#endif //IMAGE_IMAGE_H
