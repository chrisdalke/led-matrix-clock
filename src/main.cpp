#include <iostream>
#include "raylib.h"

const int texWidth = 128;
const int texHeight = 64;

const int screenZoomFactor = 10;
const int screenWidth = texWidth * screenZoomFactor;
const int screenHeight = texHeight * screenZoomFactor;


int main() {
    InitWindow(screenWidth, screenHeight, "LED Matrix Clock");
    RenderTexture2D target = LoadRenderTexture(texWidth, texHeight);

    SetTargetFPS(60);

    int x = 0;
    int y = 0;
    int velX = 1;
    int velY = 1;
    

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
    }

    CloseWindow();
    return 0;
}
