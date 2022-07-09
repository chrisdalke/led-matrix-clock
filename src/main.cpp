#include <iostream>
#include <fmt/core.h>
#include "raylib.h"
#include "matrix_driver.h"

const int texWidth = 128;
const int texHeight = 64;

const int screenZoomFactor = 10;
const int screenWidth = texWidth * screenZoomFactor;
const int screenHeight = texHeight * screenZoomFactor;


int main(int argc, char *argv[]) {
    InitWindow(screenWidth, screenHeight, "LED Matrix Clock");
    RenderTexture2D target = LoadRenderTexture(texWidth, texHeight);

    SetTargetFPS(10);

    int x = 0;
    int y = 0;
    int velX = 1;
    int velY = 1;

    MatrixDriver matrixDriver(texWidth, texHeight);


    while (!WindowShouldClose()) {
        // Handle updating clock state!

        x += velX;
        y += velY;
        if (x > texWidth || x < 0) {
            velX = -velX;
        }
        if (y > texHeight || y < 0) {
            velY = -velY;
        }
        

        BeginDrawing();

        // Render to internal buffer of same resolution as physical screen
        BeginTextureMode(target);

        ClearBackground(GRAY);
        DrawRectangle(x, y, 20, 20, BLUE);

        EndTextureMode();

        // Draw a debug UI on the software window
        DrawTextureEx(target.texture, (Vector2){0,0}, 0.0f, 1.0f, WHITE);
        DrawTexturePro(target.texture, (Rectangle){ 0, 0, texWidth, -texHeight }, (Rectangle){ 0, 0, screenWidth, screenHeight }, (Vector2){0,0}, 0.0f, WHITE); 
        DrawFPS(10,10);

        EndDrawing();

        // Grab each pixel in the texture and render to the LED matrix
        // canvas->Clear();
        Image canvasImage = LoadImageFromTexture(target.texture);
        for (int xx = 0; xx < texWidth; xx++) {
            for (int yy = 0; yy < texHeight; yy++) {
                Color pix = GetImageColor(canvasImage, xx, yy);
                matrixDriver.writePixel(xx, yy, pix.r, pix.g, pix.b);
            }
        }
        matrixDriver.flipBuffer();
    }

    CloseWindow();
    return 0;
}
