#include <iostream>
#include <locale>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <ctime>
#include <fmt/core.h>
#include "raylib.h"
#include "matrix_driver.h"

const int texWidth = 64;
const int texHeight = 32;

const int screenZoomFactor = 10;
const int screenWidth = texWidth * screenZoomFactor;
const int screenHeight = texHeight * screenZoomFactor;

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void drawOutlinedText(const char* text, int x, int y, int size, Color bg, Color fg) {
    DrawText(text, x-1, y-1, size, bg);
    DrawText(text, x-0, y-1, size, bg);
    DrawText(text, x+1, y-1, size, bg);
    DrawText(text, x-1, y-0, size, bg);
    DrawText(text, x-0, y-0, size, bg);
    DrawText(text, x+1, y-0, size, bg);
    DrawText(text, x-1, y+1, size, bg);
    DrawText(text, x-0, y+1, size, bg);
    DrawText(text, x+1, y+1, size, bg);

    DrawText(text, x, y, size, fg);
}

typedef enum WeatherType
{
    full_sun = 1,
    partial_sun = 2,
    cloudy_sun = 3,
    cloudy_rain = 4,
    cloudy_thunder = 5,
    full_moon = 6,
    partial_moon = 7,
    cloudy_moon = 8
} WeatherType;

int main(int argc, char** argv) {
    InitWindow(screenWidth, screenHeight, "LED Matrix Clock");
    RenderTexture2D target = LoadRenderTexture(texWidth, texHeight);
    MatrixDriver matrixDriver(&argc, &argv, texWidth, texHeight);

    if (matrixDriver.isShim()) {
        SetTargetFPS(30);
    } else {
        SetTargetFPS(1);
    }


    int x = 0;
    int y = 0;
    int velX = 1;
    int velY = 1;

    
    char timeBuffer[256];
    char timeBuffer2[256];
    char dateBuffer[256];

    int temperatures[24];
    int minTemperature = 60;
    int maxTemperature = 80;

    uint64_t lastWeatherQuery = 0;
    std::string rawWeatherData;

    bool dimMode = false;
    

    // Texture2D dayBg = LoadTextureFromImage(GenImageGradientV(texWidth, texHeight, (Color){0, 0, 0,255}, (Color){43, 169, 252,255}));
    // Texture2D parallaxBgImg = LoadTexture("resources/bg.png");
    int currentTemperature = 69;
    WeatherType weatherEnum = WeatherType::full_sun;


    Texture2D weatherIconCloud1 = LoadTexture("resources/weather-icon-cloud-1.png");
    Texture2D weatherIconCloud2 = LoadTexture("resources/weather-icon-cloud-2.png");
    Texture2D weatherIconCloud3 = LoadTexture("resources/weather-icon-cloud-3.png");
    Texture2D weatherIconCloud4 = LoadTexture("resources/weather-icon-cloud-4.png");
    Texture2D weatherIconMoonCloud1 = LoadTexture("resources/weather-icon-moon-cloud-1.png");
    Texture2D weatherIconSun = LoadTexture("resources/weather-icon-sun.png");
    Texture2D weatherIconMoon = LoadTexture("resources/weather-icon-moon.png");

    float timeOfDayPercent = 0.0f;

    /*
    - ring with sun, moon, sunset, stars, etc as base layer
    
     TODO: weather layers:
     - fog layer
     - rain particle layer
     - lightning particle layer
     - snow particle layer
     - cloud particle layer

     plot of temperature over the next 24 hours
     Dot showing current temperature
     make temp curve darker based on sunset/sunrise
     */

    while (!WindowShouldClose()) {
        // Query weather data if it is expired
        if (timeSinceEpochMillisec() - lastWeatherQuery > 30000) {
            std::cout << "Querying weather API..." << std::endl;
            lastWeatherQuery = timeSinceEpochMillisec();
        }

        // Debug: toggle brightness
        // On real device this is done with the hardware button
        if (IsKeyPressed(32)) {
            dimMode = !dimMode;
        }

        std::time_t now = std::time(nullptr);
        std::strftime(timeBuffer, 256, "%I:%M%p", std::localtime(&now));
        std::strftime(timeBuffer2, 256, "%I:%M", std::localtime(&now));
        std::strftime(dateBuffer, 256, "%b %e", std::localtime(&now));
        // Handle updating clock state!

        timeOfDayPercent = ((timeSinceEpochMillisec() / 100) % 1000) / 1000.0f;
 
        BeginDrawing();

        // Render to internal buffer of same resolution as physical screen
        BeginTextureMode(target);

        // DrawTexture(dayBg, 0, 0, (Color){255,255,255,255});
        ClearBackground((Color){0, 0, 0, 255});

        // Draw background parallax tex
        // DrawTexturePro(parallaxBgImg, (Rectangle){ 0, 0, 192,192 }, (Rectangle){32, 90, 192, 192}, (Vector2){96,96}, timeOfDayPercent * 360, WHITE); 

        // Draw time and date
        drawOutlinedText(timeBuffer, 2, 1, 5, (Color){0,0,0,255}, (Color){255,255,255,255});
        drawOutlinedText(dateBuffer, 2, 11, 2, (Color){0,0,0,255}, (Color){255,255,255,255});

        DrawRectangle(0, 24, 64, 32, (Color){30,30,30,255});
        drawOutlinedText(fmt::format("{}", currentTemperature).c_str(), texWidth - 17, 22, 2, (Color){0,0,0,255}, (Color){255,255,255,255});
        DrawRectangle(58, 22, 5,5, (Color){0,0,0,255});
        DrawRectangle(59, 23, 3,3, (Color){255,255,255,255});
        DrawRectangle(60, 24, 1,1, (Color){0,0,0,255});


        // make everything rendered before this half as bright
        DrawRectangle(0, 0, 64, 32, (Color){0,0,0,128});

        // Draw weather icon
        DrawTexture(weatherIconCloud1, 46, 11, (Color){255,255,255,255});
        
        drawOutlinedText(timeBuffer2, 2, 1, 5, (Color){0,0,0,255}, (Color){255,255,255,255});
        drawOutlinedText(fmt::format("{}", currentTemperature).c_str(), texWidth - 17, 22, 2, (Color){0,0,0,255}, (Color){255,255,255,255});

        // Draw temperature line for the current day
        for (int i = 0; i < 24; i++) {

        }



        EndTextureMode();

        // Draw a debug UI on the software window
        ClearBackground((Color){0, 0, 0, 255});
        if (dimMode) {
            DrawTexturePro(target.texture, (Rectangle){ 0, 0, texWidth, -texHeight }, (Rectangle){ 0, 0, screenWidth, screenHeight }, (Vector2){0,0}, 0.0f, (Color){255,255,255,100}); 
        } else {
            DrawTexturePro(target.texture, (Rectangle){ 0, 0, texWidth, -texHeight }, (Rectangle){ 0, 0, screenWidth, screenHeight }, (Vector2){0,0}, 0.0f, WHITE); 
        }

        EndDrawing();

        // Grab each pixel in the texture and render to the LED matrix
        // canvas->Clear();
        Image canvasImage = LoadImageFromTexture(target.texture);
        for (int xx = 0; xx < texWidth; xx++) {
            for (int yy = 0; yy < texHeight; yy++) {
                Color pix = GetImageColor(canvasImage, xx, yy);
                matrixDriver.writePixel(xx, texHeight - yy - 1, pix.r, pix.g, pix.b);
            }
        }
        matrixDriver.flipBuffer();
        UnloadImage(canvasImage);
    }

    CloseWindow();
    return 0;
}
