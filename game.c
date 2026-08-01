#include "raylib.h"
#include "raymath.h"
#include "game.h"
#include "screen.h"

static Rectangle middleBorder;
static Rectangle bar2Rectangle;
static Rectangle bar1Rectangle;

static const int borderThickness = 5;
static const int centerLineThickness = 5;
static const int edgeSpacing = 30;
static const int paddleThickness = 10;
static const int paddleHeight = 120;
static const int winningScore = 10;

static int player1Score;
static int player2Score;

static float ballVelocityX = 300.0f;
static const float ballRadius = 20.0f;

const float speed = 10.0f;

static Vector2 bar2Position;
static Vector2 bar1Position;
static Vector2 ballPosition;

void InitGame(void)
{
    bar1Position = (Vector2){
        (float)edgeSpacing,
        GetScreenHeight() / 2.0f};

    ballPosition = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

    player1Score = 0;
    player2Score = 0;
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

    // Update right paddle
    bar2Position = GetMousePosition();

    bar2Position.y = Clamp(
        bar2Position.y,
        paddleHeight / 2.0f + 10,
        GetScreenHeight() - paddleHeight / 2.0f - 10);

    // Update left paddle
    if (IsKeyDown(bar1Upkey))
        bar1Position.y -= 12.0f;
    if (IsKeyDown(bar1DownKey))
        bar1Position.y += 12.0f;
    bar1Position.y = Clamp(
        bar1Position.y,
        paddleHeight / 2.0f + 10,
        GetScreenHeight() - paddleHeight / 2.0f - 10);

    // Create rectanbles after positions
    bar1Rectangle = (Rectangle){
        (float)(edgeSpacing),
        bar1Position.y - paddleHeight / 2.0f,
        (float)paddleThickness,
        (float)paddleHeight};
    bar2Rectangle = (Rectangle){
        (float)(GetScreenWidth() - edgeSpacing - paddleThickness),
        bar2Position.y - paddleHeight / 2.0f,
        (float)paddleThickness,
        (float)paddleHeight};

    // Move ball
    ballPosition.x += ballVelocityX * GetFrameTime();
    if (ballVelocityX > 0 &&
        CheckCollisionCircleRec(ballPosition, ballRadius, bar2Rectangle))
    {
        ballVelocityX = -ballVelocityX;
        ballPosition.x = bar2Rectangle.x - ballRadius;
    }

    if (ballVelocityX < 0 &&
        CheckCollisionCircleRec(ballPosition, ballRadius, bar1Rectangle))
    {
        ballVelocityX = -ballVelocityX;
        ballPosition.x =
            bar1Rectangle.x + bar1Rectangle.width + ballRadius;
    }

    // Gives points to player
    if (ballPosition.x - ballRadius > GetScreenWidth())
    {
        player1Score++;
    }
    if (ballPosition.x - ballRadius < 0)
    {
        player2Score++;
    }
    if (ballPosition.x - ballRadius < 0 || ballPosition.x - ballRadius > GetScreenWidth())
    {
        ballPosition.x = GetScreenWidth() / 2;
    }

    // Displays winning score
    if (player1Score == 10)
    {
    }
    if (player2Score == 10)
    {
    }
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
    DrawRectangleRec(bar1Rectangle, WHITE);
    DrawRectangleRec(bar2Rectangle, WHITE);

    // Draw Ball
    DrawCircleV(ballPosition, ballRadius, WHITE);

    // Draw Scores
    const int fontSize = 20;
    const int scoreGap = 20;

    const char *player1Text = TextFormat("%d", player1Score);
    int player1TextWidth = MeasureText(player1Text, fontSize);

    DrawText(
        player1Text,
        centerX - scoreGap - player1TextWidth,
        20,
        fontSize,
        WHITE);

    DrawText(
        TextFormat("%d", player2Score),
        centerX + scoreGap,
        20,
        fontSize,
        WHITE);
}

void UnloadGame(void)
{
}