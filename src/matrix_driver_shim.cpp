#include "matrix_driver.h"

MatrixDriver::MatrixDriver(int argc, char *argv[], int _width, int _height) {
    std::cout << "Initializing shim matrix driver" << std::endl;

    this->width = _width;
    this->height = _height;
}

MatrixDriver::~MatrixDriver() {
    std::cout << "Destroying shim matrix driver" << std::endl;
}

void MatrixDriver::start() {
    std::cout << "Starting shim matrix driver" << std::endl;

}

void MatrixDriver::stop() {
    std::cout << "Stopping shim matrix driver" << std::endl;

}

void MatrixDriver::writePixel(int x, int y, int r, int g, int b) {
    std::cout << fmt::format("Writing shim pixel (x = {}, y = {}): {}, {}, {}", x, y, r, g, b) << std::endl;
}

void MatrixDriver::flipBuffer() {
    std::cout << "Flipping shim pixel buffer" << std::endl;
}