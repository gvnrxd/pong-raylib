#include "raylib.h"
#include "raymath.h"
#include "game.h"
#include "screen.h"

static Rectangle middleBorder;
static const int borderThickness = 5;
static const int centerLineThickness = 5;
static const int edgeSpacing = 30;
static const int paddleThickness = 10;
static const int paddleHeight = 120;

static Vector2 bar2Position;
static Vector2 bar1Position;

void InitGame(void)
{

    bar1Position = (Vector2){
        (float)edgeSpacing,
        GetScreenHeight() / 2.0f};
}

void UpdateGame(void)
{
    // Controls for left paddle
    static KeyboardKey bar1Upkey = KEY_W;
    static KeyboardKey bar1DownKey = KEY_S;

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        currentScreen = SCREEN_TITLE;
    }

    // Right paddle follows mouse
    bar2Position = GetMousePosition();
    bar2Position.y = Clamp(
        bar2Position.y,
        paddleHeight / 2.0f + 10,
        GetScreenHeight() - paddleHeight / 2.0f - 10);

    // Left paddle uses keyboard
    if (IsKeyDown(bar1Upkey))
        bar1Position.y -= 12.0f;
    if (IsKeyDown(bar1DownKey))
        bar1Position.y += 12.0f;
    bar1Position.y = Clamp(
        bar1Position.y,
        paddleHeight / 2.0f + 10,
        GetScreenHeight() - paddleHeight / 2.0f - 10);
}

void DrawGame(void)
{
    ClearBackground(BLACK);
    // Draw playing field
    float centerX = GetScreenWidth() / 2.0f;

    for (int offset = 0; offset < centerLineThickness; offset++)
    {
        float x = centerX - centerLineThickness / 2.0f + offset;
        DrawLineDashed(
            (Vector2){x, 10},
            (Vector2){x, (float)GetScreenHeight() - 5},
            20,
            5,
            WHITE);
    }

    DrawRectangle(
        0,
        5,
        GetScreenWidth(),
        borderThickness,
        WHITE);
    DrawRectangle(
        0,
        GetScreenHeight() - 10,
        GetScreenWidth(),
        borderThickness,
        WHITE);

    // Draw paddles
    DrawRectangle(
        edgeSpacing,
        (int)(bar1Position.y - paddleHeight / 2.0f),
        paddleThickness,
        paddleHeight,
        WHITE);

    DrawRectangle(
        GetScreenWidth() - edgeSpacing - paddleThickness,
        (int)(bar2Position.y - paddleHeight / 2.0f),
        paddleThickness,
        paddleHeight,
        WHITE);
}

void UnloadGame(void)
{
}