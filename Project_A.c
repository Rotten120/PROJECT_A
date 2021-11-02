#include "raylib.h"

#define SETFPS 60
#define MAX 8

const int screenWidth = 1200;
const int screenHeight = 800;

enum BlockType {CRATE, STONE, HAY, POND, BORDER};

struct Stats
{
    int speed;
    int damage;
    int health;
};

struct PlayerData
{
    Rectangle coords;
    Rectangle movement_hitbox[4];
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
    {POND, {-400, 300, 120, 120}}
};


struct PlayerData player;
Rectangle player_Source = {0, 0, 96, 120};

void Initialize();
void PlayerMovement();
void DrawObstacles(Texture2D*);
bool IsCollideEntityRecs(Rectangle*, struct Obstacles[]);

int main()
{
    InitWindow(screenWidth, screenHeight, "PROJECT_A");
    SetTargetFPS(SETFPS);

    /* TEXTURE AND SOUND LOAD SECTION OF THE CODE */
    Texture2D Area = LoadTexture("assets/TRUE BG.png");
    Texture2D Player_Animation = LoadTexture("assets/PLAYER_ANIMATION.png");
    Texture2D Blocks = LoadTexture("assets/OBSTACLES_TRUE.png");
    /* TEXTURE AND SOUND LOAD SECTION OF THE CODE */

    player.coords.width = Player_Animation.width / 4;
    player.coords.height = Player_Animation.height / 4;

    Initialize();

    Camera2D camera;
    camera.target = (Vector2){player.coords.x + (player.coords.width / 2), player.coords.y + (player.coords.height / 2)};
    camera.offset = (Vector2){screenWidth / 2, screenHeight / 2};
    camera.rotation = 0.0f;
    camera.zoom = 0.8f;

    double BaseHitBox_Height, BaseHitbox_Ypos;

    while(!WindowShouldClose())
    {
        PlayerMovement();

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
                DrawTextureRec(Player_Animation, player_Source, (Vector2){player.coords.x, player.coords.y}, WHITE);

            EndMode2D();
        EndDrawing();
    }
    
    UnloadTexture(Area);
    UnloadTexture(Blocks);
    UnloadTexture(Player_Animation);

    CloseWindow();
    return 0;
}

void Initialize()
{
    player.coords.x = (screenWidth / 2) - (player.coords.width / 2);
    player.coords.y = (screenHeight / 2) - (player.coords.height / 2);
    player.stats.damage = 15;
    player.stats.health = 100;
    player.stats.speed = 10;

    double BaseHitBox_Height = player.coords.height / 3;
    double BaseHitbox_Ypos = BaseHitBox_Height * 2;

    player.movement_hitbox[0] = (Rectangle){player.coords.x, player.coords.y - player.stats.speed + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
    player.movement_hitbox[1] = (Rectangle){player.coords.x, player.coords.y + player.stats.speed + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
    player.movement_hitbox[2] = (Rectangle){player.coords.x - player.stats.speed, player.coords.y + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
    player.movement_hitbox[3] = (Rectangle){player.coords.x + player.stats.speed, player.coords.y + BaseHitbox_Ypos, player.coords.width, BaseHitBox_Height};
}

void DrawObstacles(Texture2D* Blocks)
{
    Vector2 SourceVec = {0, 0};

    for(int i = 0; i < MAX; i++)
    {
        switch(obstacles[i].block_type)
        {
            case CRATE: SourceVec = (Vector2){0, 0}; break;
            case STONE: SourceVec = (Vector2){120, 0}; break;
            case HAY: SourceVec = (Vector2){240, 0}; break;
            case POND: SourceVec = (Vector2){360, 0}; break;
            case BORDER: break;
            default: break;
        }
        if(obstacles[i].block_type != BORDER)
            DrawTextureRec(*Blocks, (Rectangle){SourceVec.x, SourceVec.y, obstacles[i].coords.width, obstacles[i].coords.height}, 
            (Vector2){obstacles[i].coords.x, obstacles[i].coords.y}, WHITE);
    }
}

void PlayerMovement()
{
    static int framesCounter;
    static int frame;

    if(framesCounter == 8)
    {
        framesCounter = 0;
        frame = (frame == 3)? 0 : frame + 1;
        player_Source.x = player.coords.width * frame;
    }

    if(IsKeyPressed(KEY_ENTER)) player.stats.speed += 3;
    else if(IsKeyReleased(KEY_ENTER)) player.stats.speed -= 3;

    if(IsKeyDown(KEY_W) && !IsCollideEntityRecs(&player.movement_hitbox[0], obstacles))
    {
        player_Source.y = player.coords.height * 3;
        player.coords.y -= player.stats.speed;
    }
    else if(IsKeyDown(KEY_S) && !IsCollideEntityRecs(&player.movement_hitbox[1], obstacles))
    {
        player_Source.y = player.coords.height * 0;
        player.coords.y += player.stats.speed;
    }

    if(IsKeyDown(KEY_A) && !IsCollideEntityRecs(&player.movement_hitbox[2], obstacles))
    {
        player_Source.y = player.coords.height * 1;
        player.coords.x -= player.stats.speed;
    }
    else if(IsKeyDown(KEY_D) && !IsCollideEntityRecs(&player.movement_hitbox[3], obstacles))
    {
        player_Source.y = player.coords.height * 2;
        player.coords.x += player.stats.speed;
    }

    if(IsKeyReleased(KEY_A) || IsKeyReleased(KEY_S) || IsKeyReleased(KEY_W) || IsKeyReleased(KEY_D))
    {
        player_Source.x = 0;
        framesCounter = 0;
        frame = 0;
    }
    else if(IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_W) || IsKeyDown(KEY_D))
        framesCounter++;    
}

bool IsCollideEntityRecs(Rectangle* Target_Entity, struct Obstacles Target_Hitbox[])
{
    for(int i = 0; i < MAX; i++)
        if(CheckCollisionRecs(*Target_Entity, Target_Hitbox[i].coords))
            return true;
    return false;
}
