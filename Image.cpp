//
// Created by xiangweilai on 2021/4/27.
//

#include "Image.h"


double linearInterpolation(double A, double B, double ratio_Ax_AB) {
    return A + ratio_Ax_AB * (B - A);
}

RGBdata Image::getPixel(int row, int col) {
    if (row >= 0 && row < height && col >= 0 && col < width)
        return data[row][col];
    else
        return {0, 0, 0};
}

RGBdata Image::getPixelFloat(double row, double col) {
    // BiLinear Interpolation
    if (row >= 0 && row <= height - 1 && col >= 0 && col <= width - 1) {
        int r0 = int(floor(row)), r1 = int(ceil(row));
        int c0 = int(floor(col)), c1 = int(ceil(col));
        if (r0 == r1 && c0 == c1)
            return data[r0][c0];
        if (r0 == r1 && c0 != c1) {
            double R = linearInterpolation(data[r0][c0].R, data[r0][c1].R, (col - c0) / (c1 - c0));
            double G = linearInterpolation(data[r0][c0].G, data[r0][c1].G, (col - c0) / (c1 - c0));
            double B = linearInterpolation(data[r0][c0].B, data[r0][c1].B, (col - c0) / (c1 - c0));
            return {static_cast<uint8_t>(lround(R)), static_cast<uint8_t>(lround(G)), static_cast<uint8_t>(lround(B))};
        }
        if (r0 != r1 && c0 == c1) {
            double R = linearInterpolation(data[r0][c0].R, data[r1][c0].R, (row - r0) / (r1 - r0));
            double G = linearInterpolation(data[r0][c0].G, data[r1][c0].G, (row - r0) / (r1 - r0));
            double B = linearInterpolation(data[r0][c0].B, data[r1][c0].B, (row - r0) / (r1 - r0));
            return {static_cast<uint8_t>(lround(R)), static_cast<uint8_t>(lround(G)), static_cast<uint8_t>(lround(B))};
        }
        double pR = linearInterpolation(data[r0][c0].R, data[r0][c1].R, (col - c0) / (c1 - c0));
        double pG = linearInterpolation(data[r0][c0].G, data[r0][c1].G, (col - c0) / (c1 - c0));
        double pB = linearInterpolation(data[r0][c0].B, data[r0][c1].B, (col - c0) / (c1 - c0));
        double qR = linearInterpolation(data[r1][c0].R, data[r1][c1].R, (col - c0) / (c1 - c0));
        double qG = linearInterpolation(data[r1][c0].G, data[r1][c1].G, (col - c0) / (c1 - c0));
        double qB = linearInterpolation(data[r1][c0].B, data[r1][c1].B, (col - c0) / (c1 - c0));
        double R = linearInterpolation(pR, qR, (row - r0) / (r1 - r0));
        double G = linearInterpolation(pG, qG, (row - r0) / (r1 - r0));
        double B = linearInterpolation(pB, qB, (row - r0) / (r1 - r0));
        return {static_cast<uint8_t>(lround(R)), static_cast<uint8_t>(lround(G)), static_cast<uint8_t>(lround(B))};
    } else
        return {0, 0, 0};
}

Image::Image(const std::string &path) {
    auto *reader = new ByteReader(path);

    if (reader->getBytes(0, 2) == "BM") {
        // BMP
        unsigned int offset = reader->getUIntLittleEndian(10);
        width = reader->getIntLittleEndian(18);
        height = reader->getIntLittleEndian(22);

        data = new RGBdata *[height];
        unsigned int pos = offset;
        unsigned int rowBytesCount = (24 * width + 31) / 32 * 4;

        for (int i = height - 1; i >= 0; --i) {
            data[i] = new RGBdata[width];
            for (int j = 0; j < width; ++j) {
                uint8_t b = reader->getByte(pos++);
                uint8_t g = reader->getByte(pos++);
                uint8_t r = reader->getByte(pos++);
                data[i][j] = RGBdata(r, g, b);
            }
            pos += (rowBytesCount - 3 * width); // skip padding
        }
    } else if (reader->getUShortBigEndian(2) == 42) {
        // TIFF
        unsigned int ifdOffset = reader->getUIntBigEndian(4);
        unsigned int entryCount = reader->getUShortBigEndian(ifdOffset);
        for (unsigned int e = 0; e < entryCount; ++e) {
            unsigned int entryOffset = ifdOffset + 2 + 12 * e;
            uint16_t tag = reader->getUShortBigEndian(entryOffset);
            uint16_t type = reader->getUShortBigEndian(entryOffset + 2);
            if (tag == 0x0100) {
                // ImageWidth
                width = reader->getUShortBigEndian(entryOffset + 8);
            } else if (tag == 0x0101) {
                // ImageLength
                height = reader->getUShortBigEndian(entryOffset + 8);
            } else if (tag == 0x0111) {
                // StripOffsets (Pixel Data)
                uint32_t length = reader->getUIntBigEndian(entryOffset + 4);
                uint32_t valueOffset = reader->getUIntBigEndian(entryOffset + 8);

                data = new RGBdata *[height];
                unsigned int pos = valueOffset;
                for (int i = 0; i < height; ++i) {
                    data[i] = new RGBdata[width];
                    for (int j = 0; j < width; ++j) {
                        uint8_t r = reader->getByte(pos++);
                        uint8_t g = reader->getByte(pos++);
                        uint8_t b = reader->getByte(pos++);
                        data[i][j] = RGBdata(r, g, b);
                    }
                }
            }
        }
    } else {
        assert(false);
    }
    delete reader;

    bgr1DAarray = new uint8_t[width * height * 3];
    genBGR();
}

Image::Image(const Image *another) {
    height = another->height;
    width = another->width;

    data = new RGBdata *[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new RGBdata[width];
        for (int j = 0; j < width; ++j) {
            data[i][j] = another->data[i][j];
        }
    }

    bgr1DAarray = new uint8_t[width * height * 3];
    genBGR();
}

Image::Image(int h, int w) {
    height = h;
    width = w;
    data = new RGBdata *[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new RGBdata[width];
        for (int j = 0; j < width; ++j) {
            data[i][j] = {0, 0, 0};
        }
    }
    bgr1DAarray = new uint8_t[width * height * 3];
    genBGR();
}

Image::~Image() {
    for (int i = 0; i < height; ++i) {
        delete[] data[i];
    }
    delete[] data;
    delete bgr1DAarray;
}

Image *currentImage;
int currentWindow;
int argc_main;
char **argv_main;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawPixels(currentImage->width, currentImage->height, GL_BGR_EXT, GL_UNSIGNED_BYTE, currentImage->bgr1DAarray);
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27)
        glutDestroyWindow(currentWindow);
}

void Image::show(const char *title) {
    currentImage = this;
    glutInit(&argc_main, argv_main);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                  GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutInitWindowSize(width, height);
    if (title == nullptr) title = "";
    currentWindow = glutCreateWindow(title);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMainLoop();
}

void Image::translation(int dx, int dy) {
    auto new_data = new RGBdata *[height];
    for (int i = 0; i < height; ++i) {
        new_data[i] = new RGBdata[width];
        for (int j = 0; j < width; ++j) {
            new_data[i][j] = getPixel(i - dy, j - dx);
        }
    }

    for (int i = 0; i < height; ++i) {
        delete[] data[i];
    }
    delete[] data;
    data = new_data;

    genBGR();
}

void Image::rotation(double theta) {
    theta = -theta;
    auto new_data = new RGBdata *[height];
    for (int i = 0; i < height; ++i) {
        new_data[i] = new RGBdata[width];
        for (int j = 0; j < width; ++j) {
            double x = j - width * 0.5;
            double y = height * 0.5 - i;
            double cosTheta = cos(theta * PI / 180.0);
            double sinTheta = sin(theta * PI / 180.0);
            double x_ = cosTheta * x - sinTheta * y;
            double y_ = sinTheta * x + cosTheta * y;
            new_data[i][j] = getPixelFloat(height * 0.5 - y_, x_ + width * 0.5);
        }
    }

    for (int i = 0; i < height; ++i) {
        delete[] data[i];
    }
    delete[] data;
    data = new_data;

    genBGR();
}

void Image::scale(double cx, double cy) {
    int height_new = height * cy;
    int width_new = width * cx;
    auto new_data = new RGBdata *[height_new];
    for (int i = 0; i < height_new; ++i) {
        new_data[i] = new RGBdata[width_new];
        for (int j = 0; j < width_new; ++j) {
            new_data[i][j] = getPixelFloat(i / cy, j / cx);
        }
    }
    height = height_new;
    width = width_new;

    for (int i = 0; i < height; ++i) {
        delete[] data[i];
    }
    delete[] data;
    data = new_data;

    delete[] bgr1DAarray;
    bgr1DAarray = new uint8_t[width * height * 3];
    genBGR();
}

void Image::genBGR() {
    unsigned int pos_bgr1d = 0;
    for (int i = height - 1; i >= 0; --i) {
        for (int j = 0; j < width; ++j) {
            bgr1DAarray[pos_bgr1d++] = data[i][j].B;
            bgr1DAarray[pos_bgr1d++] = data[i][j].G;
            bgr1DAarray[pos_bgr1d++] = data[i][j].R;
        }
    }
}

void Image::equalizeHist() {
    auto hsl = new HSLdata *[height];
    int cnt[101] = {0};
    double sum[101] = {0.0};

    for (int i = 0; i < height; ++i) {
        hsl[i] = new HSLdata[width];
        for (int j = 0; j < width; ++j) {
            hsl[i][j] = HSLdata(data[i][j]);
            cnt[int(round(hsl[i][j].L * 100))]++;
        }
    }
    for (int i = 0; i < 101; ++i) {
        double prev = (i == 0) ? 0 : sum[i - 1];
        sum[i] = prev + cnt[i] * 1.0 / height / width;
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            hsl[i][j].L = sum[int(round(hsl[i][j].L * 100))];
            data[i][j] = RGBdata(hsl[i][j]);
        }
    }

    for (int i = 0; i < height; ++i) {
        delete[] hsl[i];
    }
    delete[] hsl;
    genBGR();
}

void Image::showFourier() {
    auto norm = Matrix<Complex>(height, width);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            norm.data[i][j] = {data[i][j].gray() / 255.0, 0};
        }
    }
    auto fft = FFT(norm);
    auto temp = Matrix<double>(height, width);
    auto visual = Matrix<uint8_t>(height, width);

    // Visualize: Log scale & Normalize range
    double val_max = std::numeric_limits<double>::min(), val_min = std::numeric_limits<double>::max();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            double val = fft.data[i][j].mod();
            val = log(val + 1);
            temp.data[i][j] = val;
            val_max = std::max(val_max, val);
            val_min = std::min(val_min, val);
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            visual.data[i][j] = uint8_t((temp.data[i][j] - val_min) / (val_max - val_min) * 255);
        }
    }
    // Visualize: Shifting the zero-frequency component to the center
    for (int i = 0; i < height / 2; ++i) {
        for (int j = 0; j < width / 2; ++j) {
            std::swap(visual.data[i][j], visual.data[i + height / 2][j + width / 2]);
            std::swap(visual.data[i + height / 2][j], visual.data[i][j + width / 2]);
        }
    }

    unsigned int pos_bgr1d = 0;
    for (int i = height - 1; i >= 0; --i) {
        for (int j = 0; j < width; ++j) {
            bgr1DAarray[pos_bgr1d++] = visual.data[i][j];
            bgr1DAarray[pos_bgr1d++] = visual.data[i][j];
            bgr1DAarray[pos_bgr1d++] = visual.data[i][j];
        }
    }
    show("Fourier");

    auto ifft = IFFT(fft);
    pos_bgr1d = 0;
    for (int i = height - 1; i >= 0; --i) {
        for (int j = 0; j < width; ++j) {
            bgr1DAarray[pos_bgr1d++] = uint8_t(ifft.data[i][j].real * 255);
            bgr1DAarray[pos_bgr1d++] = uint8_t(ifft.data[i][j].real * 255);
            bgr1DAarray[pos_bgr1d++] = uint8_t(ifft.data[i][j].real * 255);
        }
    }
    show("Inverse Fourier");
    genBGR();
}

void Image::showEdgeDetection(FILTER_TYPE type) {
    if (type == ROBERTS) {
        auto result = new uint8_t *[height];
        for (int i = 0; i < height; ++i) {
            result[i] = new uint8_t[width];
            for (int j = 0; j < width; ++j) {
                if (i == height - 1 || j == width - 1) {
                    result[i][j] = 0;
                } else {
                    result[i][j] = abs((int) data[i][j].gray() - (int) data[i + 1][j + 1].gray()) +
                                   abs((int) data[i + 1][j].gray() - (int) data[i][j + 1].gray());
                }
            }
        }
        visualizeResponse(result, height, width, "roberts");
    } else if (type == SOBEL) {
        auto result = new uint8_t *[height];
        for (int i = 0; i < height; ++i) {
            result[i] = new uint8_t[width];
            for (int j = 0; j < width; ++j) {
                if (i == height - 1 || j == width - 1 || i == 0 || j == 0) {
                    result[i][j] = 0;
                } else {
                    int s1 = ((int) data[i + 1][j - 1].gray() - (int) data[i - 1][j - 1].gray())
                             + ((int) data[i + 1][j + 1].gray() - (int) data[i - 1][j + 1].gray())
                             + ((int) data[i + 1][j].gray() - (int) data[i - 1][j].gray()) * 2;
                    int s2 = ((int) data[i - 1][j + 1].gray() - (int) data[i - 1][j - 1].gray())
                             + ((int) data[i + 1][j + 1].gray() - (int) data[i + 1][j - 1].gray())
                             + ((int) data[i][j + 1].gray() - (int) data[i][j - 1].gray()) * 2;
                    result[i][j] = (abs(s1) + abs(s2)) / 2;
                }
            }
        }
        visualizeResponse(result, height, width, "sobel");
    } else if (type == PREWITT) {
        auto result = new uint8_t *[height];
        for (int i = 0; i < height; ++i) {
            result[i] = new uint8_t[width];
            for (int j = 0; j < width; ++j) {
                if (i == height - 1 || j == width - 1 || i == 0 || j == 0) {
                    result[i][j] = 0;
                } else {
                    int s1 = ((int) data[i + 1][j - 1].gray() - (int) data[i - 1][j - 1].gray())
                             + ((int) data[i + 1][j + 1].gray() - (int) data[i - 1][j + 1].gray())
                             + ((int) data[i + 1][j].gray() - (int) data[i - 1][j].gray());
                    int s2 = ((int) data[i - 1][j + 1].gray() - (int) data[i - 1][j - 1].gray())
                             + ((int) data[i + 1][j + 1].gray() - (int) data[i + 1][j - 1].gray())
                             + ((int) data[i][j + 1].gray() - (int) data[i][j - 1].gray());
                    result[i][j] = (abs(s1) + abs(s2)) / 2;
                }
            }
        }
        visualizeResponse(result, height, width, "prewitt");
    } else if (type == LAPLACIAN) {
        auto result = new uint8_t *[height];
        for (int i = 0; i < height; ++i) {
            result[i] = new uint8_t[width];
            for (int j = 0; j < width; ++j) {
                if (i == height - 1 || j == width - 1 || i == 0 || j == 0) {
                    result[i][j] = 0;
                } else {
                    result[i][j] = abs(4 * (int) data[i][j].gray()
                                       - (int) data[i + 1][j].gray()
                                       - (int) data[i][j + 1].gray()
                                       - (int) data[i][j - 1].gray()
                                       - (int) data[i - 1][j].gray());
                }
            }
        }
        visualizeResponse(result, height, width, "laplacian");
    } else {
        assert(false);
    }
}

void visualizeResponse(uint8_t **response, int h, int w, const char *title) {
    auto *img = new Image(h, w);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            img->data[i][j] = {response[i][j], response[i][j], response[i][j]};
        }
    }
    img->genBGR();
    img->show(title);
    delete img;
}
