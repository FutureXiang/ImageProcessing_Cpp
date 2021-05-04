//
// Created by Weilai Xiang on 21/05/05.
//

#ifndef IMAGE_COLOR_H
#define IMAGE_COLOR_H

#include <iostream>
#include <algorithm>
#include <cmath>
#include <cassert>

extern double epsilon;

bool eq(double a, double b);

bool greater(double a, double b);

bool less(double a, double b);

struct HSLdata;

struct RGBdata {
    uint8_t R, G, B;

    RGBdata() = default;

    RGBdata(uint8_t r, uint8_t g, uint8_t b) : R(r), G(g), B(b) {};

    explicit RGBdata(HSLdata hsl);

    friend std::ostream &operator<<(std::ostream &output, const RGBdata &p) {
        output << "R=" << (int) p.R << ", G=" << (int) p.G << ", B=" << (int) p.B;
        return output;
    }
};

struct HSLdata {
    double H, S, L;

    HSLdata() = default;

    HSLdata(double h, double s, double l) : H(h), S(s), L(l) {};

    explicit HSLdata(RGBdata rgb);

    friend std::ostream &operator<<(std::ostream &output, const HSLdata &p) {
        output << "H=" << p.H << ", S=" << p.S << ", L=" << p.L;
        return output;
    }
};


#endif //IMAGE_COLOR_H
