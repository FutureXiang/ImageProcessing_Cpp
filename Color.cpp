//
// Created by Weilai Xiang on 21/05/05.
//

#include "Color.h"

double epsilon = 1e-5;

bool eq(double a, double b) {
    return abs(a - b) <= (std::max(abs(a), abs(b)) * epsilon);
}

bool greater(double a, double b) {
    return (a - b) > (std::max(abs(a), abs(b)) * epsilon);
}

bool less(double a, double b) {
    return (b - a) > (std::max(abs(a), abs(b)) * epsilon);
}

HSLdata::HSLdata(RGBdata rgb) {
    double r = rgb.R * 1.0 / 255.0;
    double g = rgb.G * 1.0 / 255.0;
    double b = rgb.B * 1.0 / 255.0;
    double max = std::max(std::max(r, g), b);
    double min = std::min(std::min(r, g), b);
    double c = max - min;
    L = (max + min) / 2;
    if (eq(L, 0) || eq(L, 1))
        S = 0;
    else
        S = (max - L) / std::min(L, 1 - L);
    if (eq(c, 0))
        H = 0;
    else if (eq(max, r))
        H = 60 * (0 + (g - b) / c);
    else if (eq(max, g))
        H = 60 * (2 + (b - r) / c);
    else if (eq(max, b))
        H = 60 * (4 + (r - g) / c);
    else
        assert(false);
    if (less(H, 0)) H += 360;
}

RGBdata::RGBdata(HSLdata hsl) {
    double h = hsl.H;
    double s = hsl.S;
    double l = hsl.L;
    double c = (1 - abs(2 * l - 1)) * s;
    double h_ = h / 60;
    double x = c * (1 - abs(fmod(h_, 2) - 1));
    double r, g, b;
    if ((eq(h_, 0) || greater(h_, 0)) && less(h_, 1)) {
        r = c, g = x, b = 0;
    } else if ((eq(h_, 1) || greater(h_, 1)) && less(h_, 2)) {
        r = x, g = c, b = 0;
    } else if ((eq(h_, 2) || greater(h_, 2)) && less(h_, 3)) {
        r = 0, g = c, b = x;
    } else if ((eq(h_, 3) || greater(h_, 3)) && less(h_, 4)) {
        r = 0, g = x, b = c;
    } else if ((eq(h_, 4) || greater(h_, 4)) && less(h_, 5)) {
        r = x, g = 0, b = c;
    } else if ((eq(h_, 5) || greater(h_, 5)) && less(h_, 6)) {
        r = c, g = 0, b = x;
    } else {
        r = 0, g = 0, b = 0;
    }
    double m = l - c / 2;
    R = lround(255 * (r + m));
    G = lround(255 * (g + m));
    B = lround(255 * (b + m));
}