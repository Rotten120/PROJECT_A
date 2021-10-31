#include "raylib.h"
#include "boundary.h"

#define SETFPS 60
#define MOVEMENT_SPEED 10
#define MAX 5

const int screenWidth = 800;
const int screenHeight = 800;

int main()
{
    InitWindow(screenWidth, screenHeight, "TESTING PROGRAMS");
    SetTargetFPS(SETFPS);

    Texture2D texture = LoadTexture("assets/TRUE BG.png");
    
    Rectangle player = {(screenWidth / 2) - 25, (screenHeight / 2) - 25, 50, 50};
    
    Camera2D camera;
    camera.target = (Vector2){player.x + (player.width / 2), player.y + (player.height / 2)};
    camera.offset = (Vector2){screenWidth / 2, screenHeight / 2};
    camera.rotation = 0.0f;
    camera.zoom = 0.5f;

    Rectangle Entities[MAX] = {
        {-1600, -1600, 4000, 800},  //upper boundary
        {-1600, 1600, 4000, 800},   //lower boundary
        {-1600, -800, 800, 2400},   //left-most boundary
        {1600, -800, 800, 2400},    //right-most boundary

        {100, 100, 100, 100}
    };

    Rectangle Player_Movement_Hitbox[MAX] = {
        {player.x, player.y - MOVEMENT_SPEED, player.width, player.height},
        {player.x, player.y + MOVEMENT_SPEED, player.width, player.height},
        {player.x - MOVEMENT_SPEED, player.y, player.width, player.height},
        {player.x + MOVEMENT_SPEED, player.y, player.width, player.height}
    };

    while(!WindowShouldClose())
    {
        if(IsKeyDown(KEY_W) && !IsCollideEntityRecs(Player_Movement_Hitbox[0], Entities, MAX)) player.y -= MOVEMENT_SPEED;
        else if(IsKeyDown(KEY_S) && !IsCollideEntityRecs(Player_Movement_Hitbox[1], Entities, MAX)) player.y += MOVEMENT_SPEED;
        
        if(IsKeyDown(KEY_A) && !IsCollideEntityRecs(Player_Movement_Hitbox[2], Entities, MAX)) player.x -= MOVEMENT_SPEED;
        else if(IsKeyDown(KEY_D) && !IsCollideEntityRecs(Player_Movement_Hitbox[3], Entities, MAX)) player.x += MOVEMENT_SPEED;

        camera.target = (Vector2){player.x + (player.width / 2), player.y + (player.height / 2)};

        Player_Movement_Hitbox[0] = (Rectangle){player.x, player.y - MOVEMENT_SPEED, player.width, player.height};
        Player_Movement_Hitbox[1] = (Rectangle){player.x, player.y + MOVEMENT_SPEED, player.width, player.height};
        Player_Movement_Hitbox[2] = (Rectangle){player.x - MOVEMENT_SPEED, player.y, player.width, player.height};
        Player_Movement_Hitbox[3] = (Rectangle){player.x + MOVEMENT_SPEED, player.y, player.width, player.height};

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode2D(camera);
                DrawTexture(texture, -1600, -1600, WHITE);
                DrawRectangleRec(player, RED);
                DrawRectangleRec(Entities[4], BROWN);
            EndMode2D();

        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();
    return 0;
}