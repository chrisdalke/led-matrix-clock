#include <iostream>
#include <fmt/core.h>

class MatrixDriver {
    private:
        int width;
        int height;
    public:
        MatrixDriver(int* argc, char **argv[], int width, int height);
        ~MatrixDriver();

        void start();
        void stop();

        void writePixel(int x, int y, int r, int g, int b);
        void flipBuffer();

        bool isShim();
};
