//
// Created by xiangweilai on 2021/4/27.
//

#include "Image.h"

RGBdata Image::getPixel(int row, int col) {
    if (row >= 0 && row < height && col >= 0 && col < width)
        return data[row][col];
    else
        return {0, 0, 0};
}

Image::Image(const std::string &path) {
    auto *reader = new ByteReader(path);
    assert(reader->getBytes(0, 2) == "BM");

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

void Image::show() {
    currentImage = this;
    glutInit(&argc_main, argv_main);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                  GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutInitWindowSize(width, height);
    currentWindow = glutCreateWindow("OpenGL Show Image");
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

}

void Image::scale(double cx, double cy) {

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