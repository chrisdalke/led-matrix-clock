#include "matrix_driver.h"
#include "led-matrix.h"
#include "graphics.h"

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

MatrixDriver::MatrixDriver(int _width, int _height) {
    std::cout << "Initializing matrix driver" << std::endl;

    this->width = _width;
    this->height = _height;

    RGBMatrix::Options matrix_options;
    defaults.hardware_mapping = "adafruit-hat";
    defaults.rows = 128;
    defaults.columns = 64;
    defaults.chain_length = 1;
    defaults.parallel = 1;
    defaults.show_refresh_rate = true;

    Canvas *canvas = RGBMatrix::CreateFromFlags(0, [], &defaults);
}

MatrixDriver::~MatrixDriver() {
    std::cout << "Destroying matrix driver" << std::endl;
}

void MatrixDriver::start() {
    std::cout << "Starting matrix driver" << std::endl;

}

void MatrixDriver::stop() {
    std::cout << "Stopping matrix driver" << std::endl;

}

void MatrixDriver::writePixel(int x, int y, int r, int g, int b) {
    std::cout << fmt::format("Writing pixel (x = {}, y = {}): {}, {}, {}", x, y, r, g, b) << std::endl;
    canvas->SetPixel(x,y,r,g,b);
}

void MatrixDriver::flipBuffer() {
    std::cout << "Flipping pixel buffer" << std::endl;
}