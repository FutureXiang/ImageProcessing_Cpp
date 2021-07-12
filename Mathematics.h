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

    Complex operator/(const double &other) const {
        return {real / other, imag / other};
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

Matrix<Complex> FFT(const Matrix<Complex> &mat);

Matrix<Complex> IFFT(const Matrix<Complex> &mat);

template<typename T>
Matrix<T>::Matrix(const Matrix &other) {
    height = other.height;
    width = other.width;
    data = new T *[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new T[width];
        for (int j = 0; j < width; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

template<typename T>
Matrix<T>::Matrix(int h, int w) {
    height = h;
    width = w;
    data = new T *[h];
    for (int i = 0; i < h; ++i) {
        data[i] = new T[w];
    }
}

template<typename T>
Matrix<T>::~Matrix() {
    for (int i = 0; i < height; ++i) {
        delete[] data[i];
    }
    delete[] data;
}


#endif //IMAGE_MATHEMATICS_H
