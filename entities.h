#ifndef ENTITIES_H
#define ENTITIES_H

#include "raylib.h"

typedef struct {
    Texture2D texture;
    Rectangle rect;
    int side;
} Character;

void DrawCharacter(Character *c);

#endif