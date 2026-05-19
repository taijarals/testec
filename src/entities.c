#include "entities.h"

void DrawCharacter(Character *c)
{
    DrawTexture(c->texture, c->rect.x, c->rect.y, WHITE);
}