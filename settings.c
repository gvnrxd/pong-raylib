#include "raylib.h"
#include "settings.h"
#include "screen.h"

void InitSettings(void) {};
void UpdateSettings(void)
{
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        currentScreen = SCREEN_TITLE;
    }
};
void DrawSettings(void)
{
    ClearBackground(BLACK);
    DrawText("Settings!", 100, 100, 40, WHITE);
    DrawText("Press DELETE to go back", 100, 40, 40, WHITE);
};
void UnloadSettings(void) {};
