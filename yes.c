#include <raylib.h>
#include <GameStructures.h>

#define SETFPS 60
#define MAX 8

const int screenWidth = 1200;
const int screenHeight = 800;

struct PlayerData player;
struct SpecialEffectsData effects[] = { 0 };
Texture2D *texture_ptr[] = { 0 };

struct Obstacles obstacles[] = {
    {BORDER, {-1400, -1600, 4000, 800}},
    {BORDER, {-1400, 1600, 4000, 800}},
    {BORDER, {-1600, -800, 800, 2400}},
    {BORDER, {1600, -800, 800, 2400}},

    {CRATE, {100, 100, 120, 120}},
    {STONE, {480, 500, 120, 120}},
    {HAY, {800, 400, 120, 120}},
    {POND, {-400, 300, 120, 120}}
};

void Initialize();

int main()
{
    InitWindow(screenWidth, screenHeight, "PROJECT_A");
    SetTargetFPS(SETFPS);

    Camera2D camera = {
        {player.coords.x + (player.coords.width / 2), player.coords.y + (player.coords.height / 2)},
        {screenWidth / 2, screenHeight / 2}, 0.0f, 0.8f
    };

    while(!WindowShouldClose())
    {
            BeginDrawing();
                ClearBackground(RAYWHITE);
                BeginMode2D(camera);

                EndMode2D;
            EndDrawing();
    }

    CloseWindow();
    return 0;
}

void Initialize()
{
    Vector2 textureSize = {
        texture_ptr[PLAYER_STAND]->width,
        texture_ptr[PLAYER_STAND]->height / 4
    };

    player.action = NONE;

    player.animation = (struct Animation){
        texture_ptr[PLAYER_STAND],
        {0, 120, textureSize.x, textureSize.y},
        0, 8, 0
    };

    player.coords = (Rectangle){
        (screenWidth / 2) - (player.coords.width / 2),
        (screenHeight / 2) - (player.coords.height / 2),
        textureSize.x, textureSize.y
    };

    player.stats = (struct Stats){
        10, 15,
        {100, 100},
        {100, 100}
    };

    double BaseHitBox_Height = player.coords.height / 3;
    double BaseHitBox_Ypos = BaseHitBox_Height * 2;

    player.movement_hitbox[UP] = (Rectangle){player.coords.x, player.coords.y - player.stats.speed + BaseHitBox_Ypos, player.coords.width, BaseHitBox_Height};
    player.movement_hitbox[DOWN] = (Rectangle){player.coords.x, player.coords.y + player.stats.speed + BaseHitBox_Ypos, player.coords.width, BaseHitBox_Height};
    player.movement_hitbox[LEFT] = (Rectangle){player.coords.x - player.stats.speed, player.coords.y + BaseHitBox_Ypos, player.coords.width, BaseHitBox_Height};
    player.movement_hitbox[RIGHT] = (Rectangle){player.coords.x + player.stats.speed, player.coords.y + BaseHitBox_Ypos, player.coords.width, BaseHitBox_Height};
}

