//
// Created by Weilai Xiang on 21/05/11.
//

#ifndef IMAGE_MATHEMATICS_H
#define IMAGE_MATHEMATICS_H

#include "Image.h"
#include <cmath>
#include <cassert>
#include <vector>
#include <limits>

#define PI (3.14159265)

struct Complex {
    double real;
    double imag;

    double mod() const {
        return sqrt(real * real + imag * imag);
    }

    Complex operator+(const Complex &other) const {
        return {real + other.real, imag + other.imag};
    }

    Complex operator-(const Complex &other) const {
        return {real - other.real, imag - other.imag};
    }

    Complex operator*(const Complex &other) const {
        return {real * other.real - imag * other.imag, real * other.imag + imag * other.real};
    }
};

template<typename T>
class Matrix {
public:
    T **data;
    int height;
    int width;

    Matrix<T>(const Matrix &other);

    Matrix<T>(int h, int w);

    virtual ~Matrix();
};

int binaryPower(int num);

int bitReversal(int num, int len);

Matrix<uint8_t> *FFT(const Matrix<uint8_t> &mat);


#endif //IMAGE_MATHEMATICS_H
