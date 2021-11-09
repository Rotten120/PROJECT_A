#include "raylib.h"

const int screenWidth = 500;
const int screenHeight = 500;

int main()
{
    InitWindow(screenWidth, screenHeight, "TEST");
    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        BeginDrawing();
            DrawText("Hello World", MeasureText("Hello World", 40), screenHeight - 50, 40, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}