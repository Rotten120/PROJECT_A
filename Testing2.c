#include "raylib.h"

#define SETFPS 60
const int screenWidth = 800;
const int screenHeight = 800;

int main()
{
    InitWindow(screenWidth, screenHeight, "TESTING 2 PROGRAMS");
    SetTargetFPS(SETFPS);
    Texture2D texture = LoadTexture("assets/SLASH.png");
    int framesCounter = 0;
    int frame = 0;

    while(!WindowShouldClose())
    {
        if(framesCounter == 5)
        {
            framesCounter = 0;
            frame++;
            if(frame == 4) frame = 0;
        }

        framesCounter++;
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextureRec(texture, (Rectangle){frame * 96, 0, 96, 120}, (Vector2){screenWidth / 2, screenHeight / 2}, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}