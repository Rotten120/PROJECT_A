#include "raylib.h"

bool IsCollideEntityRecs(Rectangle Target_Entity, Rectangle Entity_To_Collide[], int size)
{
    int index;
    for(index = 0; index < size; index++)
        if(CheckCollisionRecs(Target_Entity, Entity_To_Collide[index]))
            return true; 
    return false;
}