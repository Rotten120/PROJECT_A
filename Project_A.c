#include "raylib.h"
#include "GameStructures.h"

#define SETFPS 60
#define MAX 8

const int screenWidth = 1200;
const int screenHeight = 800;

int framesCounter_player;
int next_response_player;
int framesCounter_global;

struct PlayerData player;
struct SpecialEffectsData effects[] = { 0 };
Texture2D *texture_ptr[5];

bool ShowHitbox = false;

struct Obstacles obstacles[] = {
    //TYPE OF BLOCK, {x position, y position, width, height}

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

void PlayerControls();
void PlayerAnimation();
bool PlayerAttack();
bool PlayerMovement();

void DrawObstacles(Texture2D *);
bool IsCollideEntityRecs(Rectangle *, struct Obstacles[]);
bool IsHitEntityRecs(Rectangle *, struct EntitiesData[]);

int main()
{
    InitWindow(screenWidth, screenHeight, "PROJECT_A");
    SetTargetFPS(SETFPS);

    /* TEXTURE AND SOUND LOAD SECTION OF THE CODE */
    Texture2D temp = LoadTexture("assets/happy face.jpg");
    Texture2D Area = LoadTexture("assets/TRUE BG.png");
    Texture2D Player_Standing = LoadTexture("assets/PLAYER_STANDING.png");
    Texture2D Player_Walking = LoadTexture("assets/PLAYER_WALKING.png");
    Texture2D Blocks = LoadTexture("assets/OBSTACLES_TRUE.png");

    texture_ptr[TEMP] = &temp;
    texture_ptr[STAND] = &Player_Standing;
    texture_ptr[WALK] = &Player_Walking;
    texture_ptr[OBSTACLE] = &Blocks;
    texture_ptr[MAP] = &Area;
    /* TEXTURE AND SOUND LOAD SECTION OF THE CODE */

    Initialize();

    Camera2D camera;
    camera.target = (Vector2){player.coords.x + (player.coords.width / 2), player.coords.y + (player.coords.height / 2)};
    camera.offset = (Vector2){screenWidth / 2, screenHeight / 2};
    camera.rotation = 0.0f;
    camera.zoom = 0.8f;

    double BaseHitBox_Height, BaseHitbox_Ypos;

    while (!WindowShouldClose())
    {
        PlayerControls();

        if(IsKeyPressed(KEY_F3))
            ShowHitbox = !ShowHitbox;

        camera.target = (Vector2){player.coords.x + (player.coords.width / 2), player.coords.y + (player.coords.height / 2)};

        BaseHitBox_Height = player.coords.height / 3;
        BaseHitbox_Ypos = BaseHitBox_Height * 2;

        player.movement_hitbox[UP] = (Rectangle){player.coords.x, player.coords.y - player.stats.speed + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
        player.movement_hitbox[DOWN] = (Rectangle){player.coords.x, player.coords.y + player.stats.speed + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
        player.movement_hitbox[LEFT] = (Rectangle){player.coords.x - player.stats.speed, player.coords.y + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
        player.movement_hitbox[RIGHT] = (Rectangle){player.coords.x + player.stats.speed, player.coords.y + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};

        if(framesCounter_global == SETFPS)
        {

        }
            BeginDrawing();
                ClearBackground(RAYWHITE);
                BeginMode2D(camera);

                    DrawTexture(Area, -1600, -1600, WHITE);
                    DrawObstacles(&Blocks);
                    DrawTextureRec(*player.animation.texture, player.animation.sourceTexture, (Vector2){player.coords.x, player.coords.y}, WHITE);
                    
                    if(ShowHitbox)
                    {
                        DrawRectangleLinesEx(player.coords, 6, WHITE);
                        DrawRectangleLinesEx(player.movement_hitbox[UP], 6, RED);
                        DrawRectangleLinesEx(player.movement_hitbox[DOWN], 6, BLUE);
                        DrawRectangleLinesEx(player.movement_hitbox[LEFT], 6, YELLOW);
                        DrawRectangleLinesEx(player.movement_hitbox[RIGHT], 6, BLACK);
                    }

                EndMode2D();
            EndDrawing();
        framesCounter_global++;
    }

    UnloadTexture(temp);
    UnloadTexture(Area);
    UnloadTexture(Blocks);
    UnloadTexture(Player_Standing);
    UnloadTexture(Player_Walking);

    CloseWindow();
    return 0;
}

void Initialize()
{
    int textureWidth = texture_ptr[STAND]->width;
    int textureHeight = texture_ptr[STAND]->height / 4;

    player.action = NONE;

    player.animation = (struct Animation){
        texture_ptr[STAND],
        {0, 120, textureWidth, textureHeight},
        0, 8, 0
    };

    player.coords = (Rectangle){
        (screenWidth / 2) - (player.coords.width / 2),
        (screenHeight / 2) - (player.coords.height / 2),
        textureWidth,
        textureHeight
    };

    player.stats = (struct Stats){10, 15, {100, 100}, {100, 100}};

    double BaseHitBox_Height = player.coords.height / 3;
    double BaseHitbox_Ypos = BaseHitBox_Height * 2;

    player.movement_hitbox[UP] = (Rectangle){player.coords.x, player.coords.y - player.stats.speed + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
    player.movement_hitbox[DOWN] = (Rectangle){player.coords.x, player.coords.y + player.stats.speed + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
    player.movement_hitbox[LEFT] = (Rectangle){player.coords.x - player.stats.speed, player.coords.y + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
    player.movement_hitbox[RIGHT] = (Rectangle){player.coords.x + player.stats.speed, player.coords.y + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
}

void DrawObstacles(Texture2D *Blocks)
{
    Vector2 SourceVec;

    for (int i = 0; i < MAX; i++)
    {
        switch (obstacles[i].type)
        {
        case CRATE:
            SourceVec = (Vector2){0, 0};
            break;
        case STONE:
            SourceVec = (Vector2){120, 0};
            break;
        case HAY:
            SourceVec = (Vector2){240, 0};
            break;
        case POND:
            SourceVec = (Vector2){360, 0};
            break;
        case BORDER:
            break;
        default:
            break;
        }
        if (obstacles[i].type != BORDER)
            DrawTextureRec(*Blocks, (Rectangle){SourceVec.x, SourceVec.y, obstacles[i].coords.width, obstacles[i].coords.height},
                           (Vector2){obstacles[i].coords.x, obstacles[i].coords.y}, WHITE);
        if(ShowHitbox) DrawRectangleLinesEx(obstacles[i].coords, 8, RED);
    }
}

void PlayerControls()
{
    if(framesCounter_player >= player.animation.nextFrame)
    {
        framesCounter_player = 0;
        player.animation.currentFrame++;
        PlayerAnimation();
    }

    if(next_response_player >= player.animation.sizeFrame)
    {
        if(PlayerAttack())
        {
            next_response_player = 0;
            framesCounter_player = 0;
                player.action = ATTACKING;
            player.animation = (struct Animation){
                texture_ptr[STAND],
                player.animation.sourceTexture,
                0,
                10,
                50
            };
        }
        else if(PlayerMovement())
        {
            next_response_player = 0;
            framesCounter_player++;
                player.action = WALKING;
            player.animation = (struct Animation){
                texture_ptr[WALK],
                player.animation.sourceTexture,
                player.animation.currentFrame,
                8,
                0
            };
        }
        else
        {
            next_response_player = 0;
            framesCounter_player = 0;
                player.action = NONE;
            player.animation = (struct Animation){
                texture_ptr[STAND],
                player.animation.sourceTexture,
                0,
                8,
                0
            };
        }
    }
    else
    {   
        framesCounter_player++;
        next_response_player++;
    }
}

void PlayerAnimation()
{
    switch (player.action)
    {
        case WALKING:
            if(player.animation.currentFrame >= 4)
                player.animation.currentFrame = 0;
            player.animation.sourceTexture.x = player.coords.width * player.animation.currentFrame;
        break;

        case ATTACKING:
        break;

        case NONE:
            player.animation.sourceTexture.x = 0;
        break;
        default: break;
    }
}

bool PlayerAttack()
{
    if(IsKeyDown(KEY_J))
    {
        enum Direction dir = player.animation.sourceTexture.y / player.animation.sourceTexture.height;
        int textureWidth = texture_ptr[TEMP]->width / 4;
        int textureHeight = texture_ptr[TEMP]->height / 4;

        struct SpecialEffectsData sword = {
            SLASH,
            {player.coords.x, player.coords.y, textureWidth, textureHeight},
            {
                texture_ptr[TEMP],
                {0, 0, textureWidth, textureHeight},
                0, 10, 50
            }
        };

        switch(dir)
        {
            case UP:
                sword.animation.sourceTexture.y = sword.coords.height * UP;
                sword.coords.y -= sword.animation.sourceTexture.height;
            break;

            case DOWN:
                sword.animation.sourceTexture.y = sword.coords.height * DOWN;
                sword.coords.y += sword.animation.sourceTexture.height;
            break;

            case LEFT:
                sword.animation.sourceTexture.y = sword.coords.height * LEFT;
                sword.coords.x -= sword.animation.sourceTexture.height;
            break;

            case RIGHT:
                sword.animation.sourceTexture.y = sword.coords.height * RIGHT;
                sword.coords.x += sword.animation.sourceTexture.height;
            break;

            default: break;
        }
            //add that something that deals damage
        return true;
    }
    return false;
}

bool PlayerMovement()
{
    if(IsKeyDown(KEY_W) && !IsCollideEntityRecs(&player.movement_hitbox[UP], obstacles))
    {
        player.animation.sourceTexture.y = player.coords.height * UP;
        player.coords.y -= player.stats.speed;
    }
    else if(IsKeyDown(KEY_S) && !IsCollideEntityRecs(&player.movement_hitbox[DOWN], obstacles))
    {
        player.animation.sourceTexture.y = player.coords.height * DOWN;
        player.coords.y += player.stats.speed;
    }

    if(IsKeyDown(KEY_A) && !IsCollideEntityRecs(&player.movement_hitbox[LEFT], obstacles))
    {
        player.animation.sourceTexture.y = player.coords.height * LEFT;
        player.coords.x -= player.stats.speed;
    }
    else if(IsKeyDown(KEY_D) && !IsCollideEntityRecs(&player.movement_hitbox[RIGHT], obstacles))
    {
        player.animation.sourceTexture.y = player.coords.height * RIGHT;
        player.coords.x += player.stats.speed;
    }

    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_W) || IsKeyDown(KEY_D))
        return true;
    
    return false;
}

bool IsCollideEntityRecs(Rectangle *Target_Entity, struct Obstacles Target_Hitbox[])
{
    for (int i = 0; i < MAX; i++)
        if(CheckCollisionRecs(*Target_Entity, Target_Hitbox[i].coords))
            return true;
    return false;
}

bool IsHitEntityRecs(Rectangle *Target_Entity, struct EntitiesData Target_Hitbox[])
{
    for (int i = 0; i < MAX; i++)
        if(CheckCollisionRecs(*Target_Entity, Target_Hitbox[i].coords))
            return true;
    return false;
}