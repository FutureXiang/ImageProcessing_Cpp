#include "Image.h"

int main(int argc, char **argv) {
    argc_main = argc;
    argv_main = argv;

    auto *img = new Image("test.bmp");
    auto *new_img = new Image(img);
    img->show("Read Image");
    img->showFourier();
    img->equalizeHist();
    img->show("Histogram Equalization");

    new_img->translation(20, -20);
    new_img->show("Translation: right 20, up 20");
    new_img->rotation(45);
    new_img->show("Rotation: counterclockwise 45deg");
    new_img->scale(2, 0.5);
    new_img->show("Rescale: hori by 2, vert by 1/2");
    return 0;
}
