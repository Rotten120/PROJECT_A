#include "raylib.h"

#define SETFPS 60
const int screenWidth = 800;
const int screenHeight = 800;

int main()
{
    InitWindow(screenWidth, screenHeight, "TESTING 2 PROGRAMS");
    SetTargetFPS(SETFPS);
    Texture2D texture = LoadTexture("assets/PLAYER_ANIMATION.png");
    int framesCounter = 0;
    int frame = 0;

    while(!WindowShouldClose())
    {
        if(framesCounter == 10)
        {
            framesCounter = 0;
            frame++;
            if(frame == 4) frame = 0;
        }

        framesCounter++;
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTextureRec(texture, (Rectangle){frame * 96, 0, 96, 120}, (Vector2){screenWidth / 2, screenHeight / 2}, WHITE);
        DrawTextureRec(texture, (Rectangle){frame * 96, 120, 96, 120}, (Vector2){50, 50}, WHITE);
        DrawTextureRec(texture, (Rectangle){frame * 96, 240, 96, 120}, (Vector2){screenWidth - 200, screenHeight - 200}, WHITE);
        DrawTextureRec(texture, (Rectangle){frame * 96, 360, 96, 120}, (Vector2){100, screenHeight - 200}, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}