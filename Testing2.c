#include "raylib.h"

#define SETFPS 60
#define movementspeed 6

const int screenWidth = 800;
const int screenHeight = 800;

bool IsCollideEntityRecs(Rectangle Target_Entity, Rectangle Entity_To_Collide[])
{
    int index;
    for(index = 0; index < 1; index++)
        if(CheckCollisionRecs(Target_Entity, Entity_To_Collide[index]))
            break;
    return (index == 1)? false : true;
}

int main()
{
    InitWindow(screenWidth, screenHeight, "TESTING 2 PROGRAMS");
    SetTargetFPS(SETFPS);

    Rectangle Entities[] = {
        {100, 100, 200, 200}
    };

    Rectangle Player = {500, 500, 50, 50};

    Rectangle Player_hb[4] = {
        {Player.x - movementspeed, Player.y, Player.width, Player.height},
        {Player.x, Player.y - movementspeed, Player.width, Player.height},
        {Player.x + movementspeed, Player.y, Player.width, Player.height},
        {Player.x, Player.y + movementspeed, Player.width, Player.height},
    };

    while(!WindowShouldClose())
    {
        if(IsKeyDown(KEY_W) && !IsCollideEntityRecs(Player_hb[1], Entities)) Player.y -= movementspeed;
        else if(IsKeyDown(KEY_S) && !IsCollideEntityRecs(Player_hb[3], Entities)) Player.y += movementspeed;

        if(IsKeyDown(KEY_A) && !IsCollideEntityRecs(Player_hb[0], Entities)) Player.x -= movementspeed;
        else if(IsKeyDown(KEY_D) && !IsCollideEntityRecs(Player_hb[2], Entities)) Player.x += movementspeed;

        Rectangle Player_hb_temp[4] = {
            {Player.x - movementspeed, Player.y, Player.width, Player.height},
            {Player.x, Player.y - movementspeed, Player.width, Player.height},
            {Player.x + movementspeed, Player.y, Player.width, Player.height},
            {Player.x, Player.y + movementspeed, Player.width, Player.height},
        };

        Player_hb[0] = Player_hb_temp[0];
        Player_hb[1] = Player_hb_temp[1];
        Player_hb[2] = Player_hb_temp[2];
        Player_hb[3] = Player_hb_temp[3];

        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawRectangleRec(Entities[0], BLACK);
            DrawRectangleRec(Player, RED);

            DrawRectangleLinesEx(Player_hb[0], 2, BLUE);
            DrawRectangleLinesEx(Player_hb[1], 2, YELLOW);
            DrawRectangleLinesEx(Player_hb[2], 2, GREEN);
            DrawRectangleLinesEx(Player_hb[3], 2, PINK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}