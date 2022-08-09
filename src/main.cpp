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

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double seconds_since_local_midnight() {
  time_t now;
  if (time(&now) == -1) {
    return -1;
  }
  struct tm timestamp;
  if (localtime_r(&now, &timestamp) == 0) { // C23
    return -1;
  }
  timestamp.tm_isdst = -1; // Important
  timestamp.tm_hour = 0;
  timestamp.tm_min = 0;
  timestamp.tm_sec = 0;
  time_t midnight = mktime(&timestamp);
  if (midnight == -1) {
    return -1;
  }
  return difftime(now, midnight);
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

    for (int i = 0; i < 24; i++) {
        temperatures[i] = 0 + ((i%12)*5);
    }
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

    // Convert temperature as integer degree F into a table of colors
    Texture2D temperatureScaleImg = LoadTexture("resources/temperature-scale.png");
    Image colorLookupTable = LoadImageFromTexture(temperatureScaleImg);
    Color lookupColors[128];
    for (int i = 0; i < 128; i++) {
        lookupColors[i] = GetImageColor(colorLookupTable, 0, i);
    }

    int secondInDay = seconds_since_local_midnight();

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
        secondInDay = seconds_since_local_midnight();

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
        drawOutlinedText(dateBuffer, 2, 11, 1, (Color){0,0,0,255}, (Color){255,255,255,255});

        //DrawRectangle(0, 24, 64, 32, (Color){30,30,30,255});
        DrawRectangle(58, 22, 5,5, (Color){0,0,0,255});
        DrawRectangle(59, 23, 3,3, (Color){255,255,255,255});
        DrawRectangle(60, 24, 1,1, (Color){0,0,0,255});


        // make everything rendered before this half as bright
        DrawRectangle(0, 0, 64, 32, (Color){0,0,0,128});

        drawOutlinedText(timeBuffer2, 2, 1, 5, (Color){0,0,0,255}, (Color){255,255,255,255});
        
        // find max and min temperatures
        minTemperature = 999;
        maxTemperature = -999;
        for (int i = 0; i < 24; i++) {
            if (temperatures[i] < minTemperature) {
                minTemperature = temperatures[i];
            }
            if (temperatures[i] > maxTemperature) {
                maxTemperature = temperatures[i];
            }
        }

        //DrawRectangle(0, 22, 48, 8, (Color){0,0,255,255});

        // Draw temperature line for the current day
        for (int i = 0; i < 24; i++) {
            int temp_xx = 0 + (i*2.7);
            int temp = temperatures[i];
            int temp_yy = 31 - (map(temp, minTemperature, maxTemperature, 0, 16));

            Color tempColor = (Color){255,255,255,255};
            if (temp < 0) {
                tempColor = (Color){255,255,255,255};
            } else if (temp >= 128) {
                tempColor = (Color){255,50,50,255};
            } else {
                // Valid lookup
                tempColor = lookupColors[temp];
            }

            DrawLine(temp_xx+1, temp_yy, temp_xx+1, 32, Fade(tempColor, 0.25));
            DrawLine(temp_xx+2, temp_yy, temp_xx+2, 32, Fade(tempColor, 0.25));
            DrawLine(temp_xx+3, temp_yy, temp_xx+3, 32, Fade(tempColor, 0.25));

            DrawPixel(temp_xx, temp_yy,  Fade(tempColor, 0.6));
            //DrawPixel(temp_xx+1, temp_yy,  Fade(tempColor, 0.6));
        };

        // mask out some edges of temp display
        //DrawLine(1,0,1,32, (Color){0,0,0,255});
        //DrawLine(0,0,0,32, (Color){0,0,0,255});

        // Draw an icon indicating the current position in the day since midnight
        int timeOfDay_idx = map(secondInDay, 0, 86400, 0, 24);
        int timeOfDay_xx = map(secondInDay, 0, 86400, 0, (24*2.7));

        if (timeOfDay_idx >= 0 && timeOfDay_idx < 24) {
            int timeOfDay_yy = 29 - (map(temperatures[timeOfDay_idx], minTemperature, maxTemperature, 0, 7));

            DrawLine(timeOfDay_xx+1, timeOfDay_yy, timeOfDay_xx+1,  30, (Color){255,255,255,100});
            DrawPixel(timeOfDay_xx, timeOfDay_yy, (Color){255,255,255,255});
        }

        // Draw weather icon
        DrawTexture(weatherIconCloud1, 46, 2, (Color){255,255,255,255});

        // Draw temperature
        drawOutlinedText(fmt::format("{}", currentTemperature).c_str(), texWidth - 17, 22, 2, (Color){0,0,0,255}, (Color){255,255,255,255});
        drawOutlinedText(fmt::format("{}", currentTemperature).c_str(), texWidth - 17, 22, 2, (Color){0,0,0,255}, (Color){255,255,255,255});

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
