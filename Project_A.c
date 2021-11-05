#include "raylib.h"
#include "GameStructures.h"

#define SETFPS 60
#define MAX 8

const int screenWidth = 1200;
const int screenHeight = 800;

int maxEntities = 0;

int framesCounter_player;
int next_response_player;
int framesCounter_global;

struct PlayerData player;
struct SpecialEffectsData effects[] = { 0 };
Texture2D *texture_ptr[5];

struct Obstacles obstacles[] = {
    //TYPE OF BLOCK, {x position, y position, width, height}

    {BORDER, {-1400, -1600, 4000, 800}},
    {BORDER, {-1400, 1600, 4000, 800}},
    {BORDER, {-1600, -800, 800, 2400}},
    {BORDER, {1600, -800, 800, 2400}},

    {CRATE, {100, 100, 120, 120}},
    {STONE, {500, 500, 120, 120}},
    {HAY, {800, 400, 120, 120}},
    {POND, {-400, 300, 120, 120}}};

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

    player.coords.width = Player_Standing.width;
    player.coords.height = Player_Standing.height / 4;

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
    player.coords.x = (screenWidth / 2) - (player.coords.width / 2);
    player.coords.y = (screenHeight / 2) - (player.coords.height / 2);

    double BaseHitBox_Height = player.coords.height / 3;
    double BaseHitbox_Ypos = BaseHitBox_Height * 2;
    
    player.action = NONE;

    player.animation.sourceTexture = (Rectangle){0, 120, player.coords.width, player.coords.height};
    player.animation.texture = texture_ptr[STAND];
    player.animation.currentFrame = 0;
    player.animation.sizeFrame = 8;
    player.animation.nextFrame = 8;

    player.stats.damage = 15;
    player.stats.speed = 10;

    player.stats.health.max = 100;
    player.stats.stamina.max = 100;

    player.stats.health.current = player.stats.health.max;
    player.stats.stamina.current = player.stats.stamina.max;

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

        }
        else if(PlayerMovement())
        {
            next_response_player = 0;
            framesCounter_player++;
                player.animation.sizeFrame = 0;
                player.action = WALKING;
            player.animation = (struct Animation){
                texture_ptr[WALK],
                player.animation.sourceTexture,
                player.animation.currentFrame,
                8
            };
        }
        else
        {
            next_response_player = 0;
            framesCounter_player++;
                player.animation.sizeFrame = 0;
                player.action = NONE;
            player.animation = (struct Animation){
                texture_ptr[STAND],
                player.animation.sourceTexture,
                0,
                8
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
    if(IsKeyPressed(KEY_J))
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
                0, 10, 40
            }
        };

        switch(dir)
        {
            case UP:
                sword.animation.sourceTexture.y = sword.coords.height * UP;
                sword.coords.y -= textureHeight;
            break;

            case DOWN:
                sword.animation.sourceTexture.y = sword.coords.height * DOWN;
                sword.coords.y += textureHeight;
            break;

            case LEFT:
                sword.animation.sourceTexture.y = sword.coords.height * LEFT;
                sword.coords.x -= textureWidth;
            break;

            case RIGHT:
                sword.animation.sourceTexture.y = sword.coords.height * RIGHT;
                sword.coords.x += textureWidth;
            break;

            default: break;
        }

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
    else
        return false;
}

bool IsCollideEntityRecs(Rectangle *Target_Entity, struct Obstacles Target_Hitbox[])
{
    for (int i = 0; i < MAX; i++)
        if (CheckCollisionRecs(*Target_Entity, Target_Hitbox[i].coords))
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