#include "raylib.h"
#include "screen.h"
#include "title_screen.h"
#include "game.h"
#include "settings.h"

GameScreen currentScreen = SCREEN_TITLE;

int main(void)
{
    InitWindow(1280, 720, "Pong Game");
    SetTargetFPS(60);

    InitTitleScreen();
    InitGame();

    while (!WindowShouldClose())
    {
        switch (currentScreen)
        {
        case SCREEN_TITLE:
            UpdateTitleScreen();
            break;

        case SCREEN_GAME:
            UpdateGame();
            break;

        case SCREEN_SETTINGS:
            UpdateSettings();
            break;
        }

        BeginDrawing();
        ClearBackground(WHITE);

        switch (currentScreen)
        {
        case SCREEN_TITLE:
            DrawTitleScreen();
            break;

        case SCREEN_GAME:
            DrawGame();
            break;

        case SCREEN_SETTINGS:
            DrawSettings();
            break;
        }

        EndDrawing();
    }

    UnloadGame();
    CloseWindow();

    return 0;
}