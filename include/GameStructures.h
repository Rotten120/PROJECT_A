#include "raylib.h"

//enumarates the textures in the pointer
typedef enum Texture_PTR
{
    TEMP,
    STAND,
    WALK,
    OBSTACLE,
    MAP
};

//enlists entities possible
typedef enum Entities
{
    ENEMY_1
};

//enlists visual graphics included
typedef enum VisualEffects
{
    SLASH
};

//enlists obstacles included
typedef enum BlockType
{
    CRATE,
    STONE,
    HAY,
    POND,
    BORDER
};

//enumerates the 4 cardinal directions
typedef enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

//enumerates possible actions by player or entity
typedef enum Action
{
    NONE,
    WALKING,
    ATTACKING
};

//for bars like health and stamina
typedef struct Bars
{
    int current;                //defines the counting integer
    int max;                    //defines the maximum of the current
};

//defines stats of entity
typedef struct Stats
{
    int speed;                  //how many pixels does entity move per frame
    int damage;                 //damage it can inflict in health
    struct Bars health;                //defines the health
    struct Bars stamina;               //defines the stamina
};

//defines the animation of entity
typedef struct Animation
{
    Texture2D* texture;         //what texture will be used
    Rectangle sourceTexture;    //where in the texture is used

    int currentFrame;           //what currently texture is use
    int nextFrame;              //how many frames before it loads the next texture
    int sizeFrame;              //how many frames before the animation stops
};

//defines the player's parameters
typedef struct PlayerData
{
    Rectangle coords;           
    Rectangle movement_hitbox[4];

    struct Animation animation;
    enum Action action;

    struct Stats stats;
};

typedef struct SpecialEffectsData
{
    enum VisualEffects type;
    Rectangle coords;
    struct Animation animation;
};

typedef struct EntitiesData
{
    Rectangle coords;
    Rectangle movement_hitbox[4];

    struct Animation animation;
    enum Entities type;

    struct Stats stats;
};

typedef struct Obstacles
{
    enum BlockType type;
    Rectangle coords;
};
