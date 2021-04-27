#include "Image.h"

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    auto *img = new Image("test.bmp");
    img->show();
    return 0;
}
