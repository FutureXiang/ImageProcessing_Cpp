#include "Image.h"

int main(int argc, char **argv) {
    argc_main = argc;
    argv_main = argv;

    auto *img = new Image("test.bmp");
    img->show();
    auto *new_img = new Image(img);
    new_img->translation(20, -20);
    new_img->show();
    new_img->rotation(45);
    new_img->show();
    new_img->scale(2, 0.5);
    new_img->show();
    return 0;
}
