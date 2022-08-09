#include "matrix_driver.h"
#include "led-matrix.h"
#include "graphics.h"
#include <unistd.h>
#include <wiringPi.h>

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;
using rgb_matrix::FrameCanvas;

RGBMatrix* matrix;
FrameCanvas *canvas;

MatrixDriver::MatrixDriver(int* argc, char **argv[], int _width, int _height) {
    std::cout << "Initializing matrix driver" << std::endl;

    this->width = _width;
    this->height = _height;

    RGBMatrix::Options matrix_options;
    matrix_options.hardware_mapping = "adafruit-hat-pwm";
    matrix_options.rows = 32;
    matrix_options.cols = 64;
    matrix_options.chain_length = 1;
    matrix_options.parallel = 1;
    matrix_options.brightness = 100;
    matrix_options.pwm_dither_bits = 1;
    matrix_options.show_refresh_rate = false;

    matrix = RGBMatrix::CreateFromFlags(argc, argv, &matrix_options);
    canvas = matrix->CreateFrameCanvas();

    // init wiringpi
    wiringPiSetupGpio();
    pinMode(25, INPUT);
    //pullUpDnControl(6, PUD_UP);

    usleep(2000000);
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
    //std::cout << fmt::format("Writing pixel (x = {}, y = {}): {}, {}, {}", x, y, r, g, b) << std::endl;
    canvas->SetPixel(x,y,r,g,b);
}

void MatrixDriver::flipBuffer() {
    //std::cout << "Flipping pixel buffer" << std::endl;
    canvas = matrix->SwapOnVSync(canvas);
    canvas->Fill(0,0,0);
}

bool MatrixDriver::isShim() {
    return false;
}

bool MatrixDriver::hardwareSwitchPressed() {
    return digitalRead(25);
}