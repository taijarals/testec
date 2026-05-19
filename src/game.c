#include "game.h"
            ResetGame(game);
        }

        return;
    }

    if(IsKeyPressed(KEY_ONE))
    {
        if(game->hunter.side == game->wolf.side)
        {
            MoveCharacter(&game->hunter, !game->hunter.side);
            MoveCharacter(&game->wolf, !game->wolf.side);
        }
    }

    if(IsKeyPressed(KEY_TWO))
    {
        if(game->hunter.side == game->sheep.side)
        {
            MoveCharacter(&game->hunter, !game->hunter.side);
            MoveCharacter(&game->sheep, !game->sheep.side);
        }
    }

    if(IsKeyPressed(KEY_THREE))
    {
        MoveCharacter(&game->hunter, !game->hunter.side);
    }

    CheckRules(game);
}

void DrawGame(GameState *game)
{
    DrawTexture(background, 0, 0, WHITE);

    DrawCharacter(&game->hunter);
    DrawCharacter(&game->wolf);
    DrawCharacter(&game->sheep);

    DrawRectangle(520, 580, 160, 50, DARKBROWN);

    DrawText("1 - Levar lobo", 20, 20, 30, WHITE);
    DrawText("2 - Levar ovelha", 20, 60, 30, WHITE);
    DrawText("3 - Ir sozinho", 20, 100, 30, WHITE);

    if(game->gameOver)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.7f));
        DrawText("GAME OVER", 420, 300, 60, RED);
        DrawText("Pressione R para reiniciar", 350, 380, 30, WHITE);
    }

    if(game->victory)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.7f));
        DrawText("VOCE VENCEU", 390, 300, 60, GREEN);
        DrawText("Pressione R para jogar novamente", 300, 380, 30, WHITE);
    }
}

void ResetGame(GameState *game)
{
    UnloadTexture(game->hunter.texture);
    UnloadTexture(game->wolf.texture);
    UnloadTexture(game->sheep.texture);

    InitGame(game);
}