#include "matrix_driver.h"
#include "led-matrix.h"
#include "graphics.h"

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

Canvas *canvas;

MatrixDriver::MatrixDriver(int _width, int _height) {
    std::cout << "Initializing matrix driver" << std::endl;

    this->width = _width;
    this->height = _height;

    RGBMatrix::Options matrix_options;
    matrix_options.hardware_mapping = "adafruit-hat";
    matrix_options.rows = 128;
    matrix_options.columns = 64;
    matrix_options.chain_length = 1;
    matrix_options.parallel = 1;
    matrix_options.show_refresh_rate = true;

    canvas = RGBMatrix::CreateFromFlags(0, [], &matrix_options);
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