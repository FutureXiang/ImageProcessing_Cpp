//
// Created by xiangweilai on 2021/4/27.
//

#include "Image.h"

RGB Image::getPixel(int row, int col) {
    assert(row >= 0 && row < height);
    assert(col >= 0 && col < width);
    return data[row][col];
}

Image::Image(const std::string &path) {
    auto *reader = new ByteReader(path);
    assert(reader->getBytes(0, 2) == "BM");

    unsigned int offset = reader->getUIntLittleEndian(10);
    width = reader->getIntLittleEndian(18);
    height = reader->getIntLittleEndian(22);

    data = new RGB *[height];
    bgr1DAarray = new uint8_t[width * height * 3];

    unsigned int pos = offset, pos_bgr1d = 0;
    unsigned int rowBytesCount = (24 * width + 31) / 32 * 4;

    for (int i = height - 1; i >= 0; --i) {
        data[i] = new RGB[width];
        for (int j = 0; j < width; ++j) {
            char b = reader->getByte(pos++);
            char g = reader->getByte(pos++);
            char r = reader->getByte(pos++);
            data[i][j] = RGB(r, g, b);
            bgr1DAarray[pos_bgr1d++] = b;
            bgr1DAarray[pos_bgr1d++] = g;
            bgr1DAarray[pos_bgr1d++] = r;
        }
        pos += (rowBytesCount - 3 * width); // skip padding
    }

}

Image *currentImage;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawPixels(currentImage->width, currentImage->height, GL_BGR_EXT, GL_UNSIGNED_BYTE, currentImage->bgr1DAarray);
    glutSwapBuffers();
}

void Image::show() {
    currentImage = this;
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("OpenGL Show Image");
    glutDisplayFunc(display);
    glutMainLoop();
}
