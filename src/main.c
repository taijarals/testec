#include "raylib.h"
#include "game.h"
#include "ui.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lobo, Ovelha e Cacador");

    SetTargetFPS(60);

    GameState game;

    InitGame(&game);

    while(!WindowShouldClose())
    {
        UpdateGame(&game);

        BeginDrawing();

        ClearBackground(SKYBLUE);

        DrawGame(&game);
        DrawMenu();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}