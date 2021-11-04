#include "raylib.h"

#define SETFPS 60
#define MAX 8

const int screenWidth = 1200;
const int screenHeight = 800;

int framesCounter_player;
int next_response_player;
int framesCounter_global;

enum BlockType {CRATE, STONE, HAY, POND, BORDER};
enum Action {NONE, WALKING, ATTACKING};

struct Bars
{
    int current;
    int max;
};

struct Stats
{
    int speed;
    int damage;
    struct Bars health;
    struct Bars stamina;
};

struct Animation
{
    enum Action currentAction;      //defines the current action of the player
    Texture2D* texture;             //defines the texture use for the action
    Rectangle sourceTexture;        //defines where in the texture is use
    int currentFrame;               //defines which frame of the texture is use
    int nextFrame;                  //defines how long for the next animation to update
    int nextPlayerInput;            //defines how long for the next player control input
};

struct PlayerData
{
    Rectangle coords;
    Rectangle movement_hitbox[4];
    struct Animation animation;
    struct Stats stats;
};

struct Obstacles
{
    enum BlockType block_type;
    Rectangle coords;
};

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

struct PlayerData player;
Texture2D *texture_ptr[4];

void Initialize();

void PlayerControls();
bool PlayerAttack();
bool PlayerMovement();

void DrawObstacles(Texture2D *);
bool IsCollideEntityRecs(Rectangle *, struct Obstacles[]);

int main()
{
    InitWindow(screenWidth, screenHeight, "PROJECT_A");
    SetTargetFPS(SETFPS);

    /* TEXTURE AND SOUND LOAD SECTION OF THE CODE */
    Texture2D Area = LoadTexture("assets/TRUE BG.png");
    Texture2D Player_Standing = LoadTexture("assets/PLAYER_STANDING.png");
    Texture2D Player_Walking = LoadTexture("assets/PLAYER_WALKING.png");
    Texture2D Blocks = LoadTexture("assets/OBSTACLES_TRUE.png");

    texture_ptr[0] = &Area;
    texture_ptr[1] = &Blocks;
    texture_ptr[2] = &Player_Standing;
    texture_ptr[3] = &Player_Walking;
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

        player.movement_hitbox[0] = (Rectangle){player.coords.x, player.coords.y - player.stats.speed + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
        player.movement_hitbox[1] = (Rectangle){player.coords.x, player.coords.y + player.stats.speed + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
        player.movement_hitbox[2] = (Rectangle){player.coords.x - player.stats.speed, player.coords.y + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
        player.movement_hitbox[3] = (Rectangle){player.coords.x + player.stats.speed, player.coords.y + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};

            BeginDrawing();
                ClearBackground(RAYWHITE);
                BeginMode2D(camera);

                    DrawTexture(Area, -1600, -1600, WHITE);
                    DrawObstacles(&Blocks);
                    DrawTextureRec(*player.animation.texture, player.animation.sourceTexture, (Vector2){player.coords.x, player.coords.y}, WHITE);
    
                EndMode2D();
                DrawText(TextFormat("%d", player.coords.height), 50, 50, 45, BLUE);
                DrawText(TextFormat("%d", player.animation.currentAction), 50, 100, 45, BLUE);
                DrawText(TextFormat("%d", player.animation.nextPlayerInput), 50, 150, 45, BLUE);
                DrawText(TextFormat("%d", player.animation.currentFrame), 50, 200, 45, BLUE);
                DrawText(TextFormat("%d", player.animation.nextFrame), 50, 250, 45, BLUE);
                DrawText(TextFormat("%d", next_response_player), 50, 300, 45, BLUE);
                DrawText(TextFormat("%d", framesCounter_player), 50, 350, 45, BLUE);
                DrawText(TextFormat("%d", player.animation.sourceTexture.y), 50, 400, 45, BLUE);
            EndDrawing();
    }

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

    player.animation.sourceTexture = (Rectangle){0, 120, player.coords.width, player.coords.height};
    player.animation.texture = texture_ptr[2];
    player.animation.currentAction = NONE;
    player.animation.nextPlayerInput = 8;
    player.animation.currentFrame = 0;
    player.animation.nextFrame = 8;

    player.stats.damage = 15;
    player.stats.speed = 10;

    player.stats.health.max = 100;
    player.stats.stamina.max = 100;

    player.stats.health.current = player.stats.health.max;
    player.stats.stamina.current = player.stats.stamina.max;

    player.movement_hitbox[0] = (Rectangle){player.coords.x, player.coords.y - player.stats.speed + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
    player.movement_hitbox[1] = (Rectangle){player.coords.x, player.coords.y + player.stats.speed + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
    player.movement_hitbox[2] = (Rectangle){player.coords.x - player.stats.speed, player.coords.y + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
    player.movement_hitbox[3] = (Rectangle){player.coords.x + player.stats.speed, player.coords.y + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
}

void DrawObstacles(Texture2D *Blocks)
{
    Vector2 SourceVec;

    for (int i = 0; i < MAX; i++)
    {
        switch (obstacles[i].block_type)
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
        if (obstacles[i].block_type != BORDER)
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

        switch (player.animation.currentAction)
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

    if(next_response_player >= player.animation.nextPlayerInput)
    {
        if(PlayerAttack())
        {

        }
        else if(PlayerMovement())
        {
            next_response_player = 0;
            framesCounter_player++;
            player.animation = (struct Animation){
                WALKING,
                texture_ptr[3],
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
            player.animation = (struct Animation){
                NONE,
                texture_ptr[2],
                player.animation.sourceTexture,
                0,
                0,
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

bool PlayerAttack()
{
    return false;
}

bool PlayerMovement()
{
    if(IsKeyDown(KEY_W) && !IsCollideEntityRecs(&player.movement_hitbox[0], obstacles))
    {
        player.animation.sourceTexture.y = 0;
        player.coords.y -= player.stats.speed;
    }
    else if(IsKeyDown(KEY_S) && !IsCollideEntityRecs(&player.movement_hitbox[1], obstacles))
    {
        player.animation.sourceTexture.y = player.coords.height;
        player.coords.y += player.stats.speed;
    }

    if(IsKeyDown(KEY_A) && !IsCollideEntityRecs(&player.movement_hitbox[2], obstacles))
    {
        player.animation.sourceTexture.y = player.coords.height * 2;
        player.coords.x -= player.stats.speed;
    }
    else if(IsKeyDown(KEY_D) && !IsCollideEntityRecs(&player.movement_hitbox[3], obstacles))
    {
        player.animation.sourceTexture.y = player.coords.height * 3;
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
