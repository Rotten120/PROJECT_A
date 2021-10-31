#include "raylib.h"

#define SETFPS 60

const int screenWidth = 800;
const int screenHeight = 800;

int main()
{
    InitWindow(screenWidth, screenHeight, "TESTING PROGRAMS");
    SetTargetFPS(SETFPS);

    while(!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);

            /* code */

        EndDrawing();
    }

    CloseWindow();
    return 0;
}