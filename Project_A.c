#include "raylib.h"
#include "GameStructures.h"

#define SETFPS 60
#define MAX 8

const int screenWidth = 1200;
const int screenHeight = 800;

double BaseHitBox_Height, BaseHitbox_Ypos;
int framesCounter_global;

struct PlayerData player;
struct SpecialEffectsData effects[] = { 0 };
Texture2D *texture_ptr[6];

struct Obstacles obstacles[] = {
    //TYPE OF BLOCK, {x position, y position, width, height}

    {BLOCK_BORDER, {-1400, -1600, 4000, 800}},
    {BLOCK_BORDER, {-1400, 1600, 4000, 800}},
    {BLOCK_BORDER, {-1600, -800, 800, 2400}},
    {BLOCK_BORDER, {1600, -800, 800, 2400}},

    {BLOCK_CRATE, {100, 100, 120, 120}},
    {BLOCK_STONE, {480, 500, 120, 120}},
    {BLOCK_HAY, {800, 400, 120, 120}},
    {BLOCK_POND, {-400, 300, 120, 120}}
};

void Initialize();

void DoAnimation(struct Animation*);

void PlayerControls();
bool PlayerAttack();
bool PlayerMovement();

void DrawObstacles();
Vector2 TextureSize(Texture2D*);
bool IsCollideEntityRecs(Rectangle, struct Obstacles[]);

void UnloadData();

int main()
{
    InitWindow(screenWidth, screenHeight, "PROJECT_A");
    SetTargetFPS(SETFPS);

    /* TEXTURE AND SOUND LOAD SECTION OF THE CODE */
    Texture2D
    temp = LoadTexture("assets/happy face.jpg"),
    Slash = LoadTexture("assets/SLASH.png"),
    Player_Standing = LoadTexture("assets/PLAYER_STANDING.png"),
    Player_Walking = LoadTexture("assets/PLAYER_WALKING.png"),
    Blocks = LoadTexture("assets/OBSTACLES_TRUE.png"),
    Area = LoadTexture("assets/TRUE BG.png");

    texture_ptr[TEXTURE_TEMP] = &temp;
    texture_ptr[TEXTURE_SLASH] = &Slash;
    texture_ptr[TEXTURE_STAND] = &Player_Standing;
    texture_ptr[TEXTURE_WALK] = &Player_Walking;
    texture_ptr[TEXTURE_OBSTACLE] = &Blocks;
    texture_ptr[TEXTURE_MAP] = &Area;
    /* TEXTURE AND SOUND LOAD SECTION OF THE CODE */

    Initialize();

    Camera2D camera;
    camera.target = (Vector2){player.coords.x + (player.coords.width / 2), player.coords.y + (player.coords.height / 2)};
    camera.offset = (Vector2){screenWidth / 2, screenHeight / 2};
    camera.rotation = 0.0f;
    camera.zoom = 0.8f;

    while (!WindowShouldClose())
    {
        PlayerControls();

        camera.target = (Vector2){player.coords.x + (player.coords.width / 2), player.coords.y + (player.coords.height / 2)};

        BaseHitBox_Height = player.coords.height / 3;
        BaseHitbox_Ypos = BaseHitBox_Height * 2;

        player.movement_hitbox[DIR_UP] = (Rectangle){player.coords.x, player.coords.y - player.stats.speed + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
        player.movement_hitbox[DIR_DOWN] = (Rectangle){player.coords.x, player.coords.y + player.stats.speed + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
        player.movement_hitbox[DIR_LEFT] = (Rectangle){player.coords.x - player.stats.speed, player.coords.y + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
        player.movement_hitbox[DIR_RIGHT] = (Rectangle){player.coords.x + player.stats.speed, player.coords.y + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};

            BeginDrawing();
                ClearBackground(RAYWHITE);
                BeginMode2D(camera);

                    DrawTexture(Area, -1600, -1600, WHITE);
                    DrawObstacles();
                    DrawTextureRec(*player.animation.texture, player.animation.sourceTexture, (Vector2){player.coords.x, player.coords.y}, WHITE);

                EndMode2D();
                    DrawText(TextFormat("%d", player.coords.height), 50, 50, 45, BLUE);
                    DrawText(TextFormat("%d", player.action_type), 50, 100, 45, BLUE);
                    DrawText(TextFormat("%d", player.animation.sizeFrame), 50, 150, 45, BLUE);
                    DrawText(TextFormat("%d", player.animation.currentFrame), 50, 200, 45, BLUE);
                    DrawText(TextFormat("%d", player.animation.nextFrame), 50, 250, 45, BLUE);
                    DrawText(TextFormat("%d", player.animation.nextFrame_counter), 50, 300, 45, BLUE);
                    DrawText(TextFormat("%d", player.animation.sizeFrame_counter), 50, 350, 45, BLUE);
                    DrawText(TextFormat("%d", player.animation.sourceTexture.y), 50, 400, 45, BLUE);
            EndDrawing();
    }

    UnloadData();
    CloseWindow();
    return 0;
}

void Initialize()
{
    int textureWidth = TextureSize(texture_ptr[TEXTURE_STAND]).x;
    int textureHeight = TextureSize(texture_ptr[TEXTURE_STAND]).y;

    player.action_type = ACTION_NONE;

    player.animation = (struct Animation){
        texture_ptr[TEXTURE_STAND],
        {0, 120, textureWidth, textureHeight},
        0, 8, 0, 0, 0
    };

    player.coords = (Rectangle){
        (screenWidth / 2) - (player.coords.width / 2),
        (screenHeight / 2) - (player.coords.height / 2),
        textureWidth,
        textureHeight
    };

    player.stats = (struct Stats){
        10, 15, 
        {100, 100}, 
        {100, 100}
    };

    BaseHitBox_Height = player.coords.height / 3;
    BaseHitbox_Ypos = BaseHitBox_Height * 2;

    player.movement_hitbox[DIR_UP] = (Rectangle){player.coords.x, player.coords.y - player.stats.speed + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
    player.movement_hitbox[DIR_DOWN] = (Rectangle){player.coords.x, player.coords.y + player.stats.speed + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
    player.movement_hitbox[DIR_LEFT] = (Rectangle){player.coords.x - player.stats.speed, player.coords.y + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
    player.movement_hitbox[DIR_RIGHT] = (Rectangle){player.coords.x + player.stats.speed, player.coords.y + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
}

void DrawObstacles()
{
    Vector2 SourceVec;

    for (int i = 0; i < MAX; i++)
    {
        switch (obstacles[i].type)
        {
            case BLOCK_CRATE: SourceVec = (Vector2){0, 0}; break;
            case BLOCK_STONE: SourceVec = (Vector2){120, 0}; break;
            case BLOCK_HAY: SourceVec = (Vector2){240, 0}; break;
            case BLOCK_POND: SourceVec = (Vector2){360, 0}; break;
            case BLOCK_BORDER: break;
            default: break;
        }

        if (obstacles[i].type != BLOCK_BORDER)
        {
            DrawTextureRec(
                *texture_ptr[TEXTURE_OBSTACLE],
                (Rectangle){SourceVec.x, SourceVec.y, obstacles[i].coords.width, obstacles[i].coords.height},
                (Vector2){obstacles[i].coords.x, obstacles[i].coords.y},
                WHITE
            );
        }
    }
}

/**********************************/

void DoAnimation(struct Animation* animation)
{
    if(animation->sizeFrame_counter >= animation->sizeFrame)
    {
        if(animation->nextFrame_counter >= animation->nextFrame)
        {
            animation->nextFrame_counter = 0;

            animation->currentFrame++;
            if(animation->currentFrame >= (animation->texture->width / animation->sourceTexture.width))
                animation->currentFrame = 0;

            animation->sourceTexture.x = animation->sourceTexture.width * animation->currentFrame;
        }
    }
    else
    {
        animation->sizeFrame_counter++;
        animation->nextFrame_counter++;
    }
}

/**********************************/

void PlayerControls()
{
    DoAnimation(&player.animation);

    if(player.animation.sizeFrame_counter >= player.animation.sizeFrame)
    {
        if(PlayerAttack())
        {
            
        }
        else if(PlayerMovement())
        {
            player.animation = (struct Animation){
                texture_ptr[TEXTURE_WALK],
                player.animation.sourceTexture,
                player.animation.currentFrame,
                8, 0, player.animation.nextFrame_counter + 1, 0
            };
        }
        else
        {
            player.animation = (struct Animation){
                texture_ptr[TEXTURE_WALK],
                player.animation.sourceTexture,
                0, 8, 0, 0, 0
            };
        }
    }
}

bool PlayerAttack()
{
    if(IsKeyDown(KEY_J))
    {

        return true;
    }
    return false;
}

bool PlayerMovement()
{
    if(IsKeyDown(KEY_W) && !IsCollideEntityRecs(player.movement_hitbox[DIR_UP], obstacles))
    {
        player.animation.sourceTexture.y = player.coords.height * DIR_UP;
        player.coords.y -= player.stats.speed;
    }
    else if(IsKeyDown(KEY_S) && !IsCollideEntityRecs(player.movement_hitbox[DIR_DOWN], obstacles))
    {
        player.animation.sourceTexture.y = player.coords.height * DIR_DOWN;
        player.coords.y += player.stats.speed;
    }

    if(IsKeyDown(KEY_A) && !IsCollideEntityRecs(player.movement_hitbox[DIR_LEFT], obstacles))
    {
        player.animation.sourceTexture.y = player.coords.height * DIR_LEFT;
        player.coords.x -= player.stats.speed;
    }
    else if(IsKeyDown(KEY_D) && !IsCollideEntityRecs(player.movement_hitbox[DIR_RIGHT], obstacles))
    {
        player.animation.sourceTexture.y = player.coords.height * DIR_RIGHT;
        player.coords.x += player.stats.speed;
    }

    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_W) || IsKeyDown(KEY_D))
        return true;

    return false;
}

/**********************************/

Vector2 TextureSize(Texture2D* texture)
{
    if(texture == texture_ptr[TEXTURE_SLASH] ||
       texture == texture_ptr[TEXTURE_STAND] ||
       texture == texture_ptr[TEXTURE_WALK]    )
    return (Vector2){96, 120};

    return (Vector2){0, 0};
}

bool IsCollideEntityRecs(Rectangle Target_Entity, struct Obstacles Target_Hitbox[])
{
    for (int i = 0; i < MAX; i++)
        if(CheckCollisionRecs(Target_Entity, Target_Hitbox[i].coords))
            return true;
    return false;
}

void UnloadData()
{
    for(int i = 0; i < TEXTURE_MAP; i++)
        UnloadTexture(*texture_ptr[i]);
}

