#include "raylib.h"
#include "game.h"
#include "screen.h"

void InitGame(void)
{
}

void UpdateGame(void)
{
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        currentScreen = SCREEN_TITLE;
    }
}

void DrawGame(void)
{
    ClearBackground(BLACK);
    DrawText("Game Started!", 100, 100, 40, WHITE);
    DrawText("Press DELETE to go back", 100, 40, 40, WHITE);
}

void UnloadGame(void)
{
}