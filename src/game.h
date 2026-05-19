#ifndef GAME_H
#define GAME_H

#include "entities.h"

#define LEFT 0
#define RIGHT 1

typedef struct {
    Character hunter;
    Character wolf;
    Character sheep;

    int boatSide;
    int gameOver;
    int victory;
} GameState;

void InitGame(GameState *game);
void UpdateGame(GameState *game);
void DrawGame(GameState *game);
void ResetGame(GameState *game);

#endif