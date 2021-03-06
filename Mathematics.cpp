//
// Created by Weilai Xiang on 21/05/11.
//

#include "Mathematics.h"

int binaryPower(int num) {
    int len = 0;
    while (num != 1) {
        if (num % 2) return -1;
        num /= 2;
        len++;
    }
    return len;
}

int bitReversal(int num, int len) {
    unsigned int res = 0;
    for (int i = 0; i < len; ++i) {
        res = (res << 1) | (num & 1);
        num >>= 1;
    }
    return res;
}

Matrix<Complex> FFT(const Matrix<Complex> &mat) {
    int H = mat.height, W = mat.width;
    auto result = Matrix<Complex>(H, W);

    int numH = binaryPower(H), numW = binaryPower(W);
    assert (numH != -1 && numW != -1);

    // Bit Reversing
    // https://connorg93.wordpress.com/2013/11/07/fft-bit-reversing-and-butterfly/
    auto indexRev_height = new int[H];
    auto indexRev_width = new int[W];
    for (int i = 0; i < H; ++i) {
        indexRev_height[i] = bitReversal(i, numH);
    }
    for (int i = 0; i < W; ++i) {
        indexRev_width[i] = bitReversal(i, numW);
    }
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            result.data[i][j] = mat.data[indexRev_height[i]][indexRev_width[j]];
        }
    }
    delete[] indexRev_height;
    delete[] indexRev_width;

    // Twiddle Factor
    auto W_height = new Complex[H / 2];
    for (int i = 0; i < H / 2; ++i) {
        W_height[i] = {cos(-2 * PI * i / H), sin(-2 * PI * i / H)};
    }
    auto W_width = new Complex[W / 2];
    for (int i = 0; i < W / 2; ++i) {
        W_width[i] = {cos(-2 * PI * i / W), sin(-2 * PI * i / W)};
    }

    // 1. 1D-FFT for each row
    for (int r = 0; r < H; ++r) {
        for (int i = 0; i < numW; ++i) {
            int halfSize = 1 << i, size = halfSize * 2;
            std::vector<Complex> row;
            row.reserve(W);
            for (int start = 0; start < W; start += size) {
                for (int innerPos = 0; innerPos < size; ++innerPos) {
                    if (innerPos < halfSize) {
                        auto res = result.data[r][start + innerPos] +
                                   result.data[r][start + innerPos + halfSize] * W_width[innerPos * W / size];
                        row.push_back(res);
                    } else {
                        auto res = result.data[r][start + innerPos - halfSize] -
                                   result.data[r][start + innerPos] * W_width[(innerPos - halfSize) * W / size];
                        row.push_back(res);
                    }
                }
            }
            for (int j = 0; j < W; ++j) {
                result.data[r][j] = row[j];
            }
        }
    }
    // 2. 1D-FFT for each column
    for (int c = 0; c < W; ++c) {
        for (int i = 0; i < numH; ++i) {
            int halfSize = 1 << i, size = halfSize * 2;
            std::vector<Complex> col;
            col.reserve(H);
            for (int start = 0; start < H; start += size) {
                for (int innerPos = 0; innerPos < size; ++innerPos) {
                    if (innerPos < halfSize) {
                        auto res = result.data[start + innerPos][c] +
                                   result.data[start + innerPos + halfSize][c] * W_height[innerPos * H / size];
                        col.push_back(res);
                    } else {
                        auto res = result.data[start + innerPos - halfSize][c] -
                                   result.data[start + innerPos][c] * W_height[(innerPos - halfSize) * H / size];
                        col.push_back(res);
                    }
                }
            }
            for (int j = 0; j < H; ++j) {
                result.data[j][c] = col[j];
            }
        }
    }

    delete[] W_height;
    delete[] W_width;
    return result;
}

Matrix<Complex> IFFT(const Matrix<Complex> &mat) {
    int H = mat.height, W = mat.width;

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            mat.data[i][j].imag = -mat.data[i][j].imag;
        }
    }
    auto result = FFT(mat);
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            result.data[i][j].real = result.data[i][j].real / (H * W);
            result.data[i][j].imag = -result.data[i][j].imag / (H * W);
        }
    }
    return result;
}
