#include "Image.h"
#include "Descriptor.h"

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

    auto *bin = new Image("test_fft.bmp");
    auto *bin_1 = new Image(bin);
    bin_1->translation(-40, -40);
    auto *bin_2 = new Image("test_fft_big.bmp");
    auto *bin_3 = new Image("test_fft_small.bmp");
    bin->show("original");
    bin->showFourier();
    bin_1->show("translation");
    bin_1->showFourier();
    bin_2->show("zoom in");
    bin_2->showFourier();
    bin_3->show("zoom out");
    bin_3->showFourier();

    auto edge = getEdgePoints();
    plotPoints(edge, "N = 64");
    auto descriptor = getFourierDescriptors(edge);
    plotPoints(getIFourierRecon(descriptor, 2), "M = 2");
    plotPoints(getIFourierRecon(descriptor, 32), "M = 32");
    plotPoints(getIFourierRecon(descriptor, 62), "M = 62");
    return 0;
}
