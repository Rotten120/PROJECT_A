#include "raylib.h"

char Characters[36] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'
};

const char* generate_random_ID()
{
    char output[7];
    for(int i = 0; i < 7; i++)
        output[i] = Characters[GetRandomValue(0, 35)];
    return output;
}