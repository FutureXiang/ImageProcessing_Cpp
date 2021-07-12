//
// Created by Weilai Xiang on 21/07/12.
//

#include "Descriptor.h"

const int N = 64;

ComplexPoints getEdgePoints() {
    double margin = 1.0 / (N / 4.0);
    auto res = ComplexPoints();
    res.push_back({-0.5, -0.5});
    for (int i = 1; i < N; ++i) {
        if (i <= N / 4) {
            auto real = res.back().real + margin;
            auto imag = res.back().imag;
            res.push_back({real, imag});
        } else if (i <= N / 2) {
            auto real = res.back().real;
            auto imag = res.back().imag + margin;
            res.push_back({real, imag});
        } else if (i <= N / 4 * 3) {
            auto real = res.back().real - margin;
            auto imag = res.back().imag;
            res.push_back({real, imag});
        } else {
            auto real = res.back().real;
            auto imag = res.back().imag - margin;
            res.push_back({real, imag});
        }
    }
    return res;
}

ComplexPoints getFourierDescriptors(ComplexPoints &points) {
    auto res = ComplexPoints();
    for (int u = 0; u < N; ++u) {
        Complex t = {0, 0};
        for (int k = 0; k < N; ++k) {
            Complex exp = {cos(-2 * PI * u * k / N), sin(-2 * PI * u * k / N)};
            t = t + points[k] * exp;
        }
        res.push_back(t / N);
    }
    return res;
}

ComplexPoints getIFourierRecon(ComplexPoints &desc, int M) {
    assert(M <= N - 1);
    auto res = ComplexPoints();
    for (int k = 0; k < N; ++k) {
        Complex t = {0, 0};
        for (int u = 0; u < M; ++u) {
            Complex exp = {cos(2 * PI * u * k / N), sin(2 * PI * u * k / N)};
            t = t + desc[u] * exp;
        }
        res.push_back(t);
    }
    return res;
}

void plotPoints(const ComplexPoints &points, const char *title) {
    const int size = 256;
    Image img = Image(size + 4, size + 4);
    for (auto &p: points) {
        int x = int(round((p.real + 0.5) * (size - 1)));
        int y = int(round((p.imag + 0.5) * (size - 1)));
        x = std::min(std::max(x, 0), size - 1);
        y = std::min(std::max(y, 0), size - 1);
        img.data[x][y] = {255, 255, 255};
    }
    img.genBGR();
    img.show(title);
}