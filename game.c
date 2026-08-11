#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stddef.h>
#include "game.h"
#include "screen.h"

#define MAX_BOUNCE_ANGLE (75.0f * DEG2RAD)

static Rectangle bar2Rectangle;
static Rectangle bar1Rectangle;

static const int borderThickness = 5;
static const int centerLineThickness = 5;
static const int edgeSpacing = 30;
static const int paddleThickness = 10;
static const int paddleHeight = 120;
static const int winningScore = 2;

static const float roundCountdown = 3.0f;
static const float gameCountdown = 3.0f;

static int player1Score;
static int player2Score;

static float ballTimer;
static float defaultVelocity = 900.0f;
static float ballVelocityX = 900.0f;
static float ballVelocityY = 0.0f;
static const float ballRadius = 20.0f;

static bool gameOver;
static const char *gameWinningText;

static Vector2 bar2Position;
static Vector2 bar1Position;
static Vector2 ballPosition;

void InitGame(void)
{
    bar1Position = (Vector2){
        (float)edgeSpacing,
        GetScreenHeight() / 2.0f};

    bar2Position = (Vector2){
        GetScreenWidth() - edgeSpacing - paddleThickness,
        GetScreenHeight() / 2.0f};

    ballPosition = (Vector2){
        GetScreenWidth() / 2.0f,
        GetScreenHeight() / 2.0f};

    player1Score = 0;
    player2Score = 0;

    ballVelocityX = defaultVelocity;
    ballVelocityY = 0.0f;

    ballTimer = gameCountdown;

    gameWinningText = NULL;
    gameOver = false;
}

static void BounceFromPaddle(Rectangle paddle, float direction)
{
    float paddleCenterY = paddle.y + paddle.height / 2.0f;

    float relativeIntersectY = paddleCenterY - ballPosition.y;

    float normalizedRelativeIntersectionY = relativeIntersectY / (paddle.height / 2.0f);

    normalizedRelativeIntersectionY = Clamp(normalizedRelativeIntersectionY, -1.0f, 1.0f);

    float bounceAngle = normalizedRelativeIntersectionY * MAX_BOUNCE_ANGLE;

    float ballSpeed = sqrtf(ballVelocityX * ballVelocityX + ballVelocityY * ballVelocityY);

    ballVelocityX = direction * ballSpeed * cosf(bounceAngle);

    ballVelocityY = -ballSpeed * sinf(bounceAngle);
}

void UpdateGame(void)
{
    const KeyboardKey bar1UpKey = KEY_W;
    const KeyboardKey bar1DownKey = KEY_S;

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        currentScreen = SCREEN_TITLE;
    }

    // Restart game
    if (IsKeyPressed(KEY_R))
    {
        gameOver = false;

        player1Score = 0;
        player2Score = 0;

        ballPosition = (Vector2){
            GetScreenWidth() / 2.0f,
            GetScreenHeight() / 2.0f};

        ballVelocityX = defaultVelocity;
        ballVelocityY = 0.0f;
        ballTimer = gameCountdown;
        gameWinningText = NULL;
    }

    // Update right paddle
    if (!gameOver)
    {
        bar2Position = GetMousePosition();
    }

    bar2Position.y = Clamp(
        bar2Position.y,
        paddleHeight / 2.0f + 10.0f,
        GetScreenHeight() - paddleHeight / 2.0f - 10.0f);

    // Update left paddle
    if (!gameOver)
    {
        if (IsKeyDown(bar1UpKey))
        {
            bar1Position.y -= 12.0f;
        }

        if (IsKeyDown(bar1DownKey))
        {
            bar1Position.y += 12.0f;
        }
    }

    bar1Position.y = Clamp(
        bar1Position.y,
        paddleHeight / 2.0f + 10.0f,
        GetScreenHeight() - paddleHeight / 2.0f - 10.0f);

    // Create paddle rectangles
    bar1Rectangle = (Rectangle){
        (float)edgeSpacing,
        bar1Position.y - paddleHeight / 2.0f,
        (float)paddleThickness,
        (float)paddleHeight};

    bar2Rectangle = (Rectangle){
        (float)(GetScreenWidth() - edgeSpacing - paddleThickness),
        bar2Position.y - paddleHeight / 2.0f,
        (float)paddleThickness,
        (float)paddleHeight};

    // Update countdown
    if (!gameOver && ballTimer > 0.0f)
    {
        ballTimer -= GetFrameTime();

        if (ballTimer < 0.0f)
        {
            ballTimer = 0.0f;
        }
    }

    // Move ball after countdown
    if (!gameOver && ballTimer <= 0.0f)
    {
        ballPosition.x += ballVelocityX * GetFrameTime();
        ballPosition.y += ballVelocityY * GetFrameTime();

        // Bounce off the top border
        if (ballPosition.y - ballRadius <= 10.0f)
        {
            ballPosition.y = 10.0f + ballRadius;
            ballVelocityY = fabsf(ballVelocityY);
        }

        // Bounce off the bottom border
        if (ballPosition.y + ballRadius >= GetScreenHeight() - 10.0f)
        {
            ballPosition.y =
                GetScreenHeight() - 10.0f - ballRadius;
            ballVelocityY = -fabsf(ballVelocityY);
        }

        if (ballVelocityX > 0.0f &&
            CheckCollisionCircleRec(
                ballPosition,
                ballRadius,
                bar2Rectangle))
        {
            ballPosition.x = bar2Rectangle.x - ballRadius;
            BounceFromPaddle(bar2Rectangle, -1.0f);
        }

        if (ballVelocityX < 0.0f &&
            CheckCollisionCircleRec(
                ballPosition,
                ballRadius,
                bar1Rectangle))
        {
            ballPosition.x =
                bar1Rectangle.x +
                bar1Rectangle.width +
                ballRadius;
            BounceFromPaddle(bar1Rectangle, 1.0f);
        }
    }

    // Player 1 scores
    if (ballPosition.x - ballRadius > GetScreenWidth())
    {
        player1Score++;

        ballPosition = (Vector2){
            GetScreenWidth() / 2.0f,
            GetScreenHeight() / 2.0f};

        ballVelocityX = -defaultVelocity;
        ballVelocityY = 0.0f;
        ballTimer = roundCountdown;
    }

    // Player 2 scores
    if (ballPosition.x + ballRadius < 0.0f)
    {
        player2Score++;

        ballPosition = (Vector2){
            GetScreenWidth() / 2.0f,
            GetScreenHeight() / 2.0f};

        ballVelocityX = defaultVelocity;
        ballVelocityY = 0.0f;
        ballTimer = roundCountdown;
    }

    // Handle game over
    if (player1Score >= winningScore)
    {
        gameOver = true;
        gameWinningText = "Player 1 wins";
    }
    else if (player2Score >= winningScore)
    {
        gameOver = true;
        gameWinningText = "Player 2 wins";
    }
}

void DrawGame(void)
{
    ClearBackground(BLACK);

    float centerX = GetScreenWidth() / 2.0f;

    // Draw center line
    for (int offset = 0; offset < centerLineThickness; offset++)
    {
        float x =
            centerX -
            centerLineThickness / 2.0f +
            offset;

        DrawLineDashed(
            (Vector2){x, 10.0f},
            (Vector2){
                x,
                (float)GetScreenHeight() - 5.0f},
            20.0f,
            5.0f,
            WHITE);
    }

    // Draw borders
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

    // Draw paddles and ball
    DrawRectangleRec(bar1Rectangle, WHITE);
    DrawRectangleRec(bar2Rectangle, WHITE);
    DrawCircleV(ballPosition, ballRadius, WHITE);

    // Draw scores
    const int fontSize = 20;
    const int largeFontSize = fontSize * 3;
    const int scoreGap = 20;

    const char *player1Text = TextFormat("%d", player1Score);
    int player1TextWidth =
        MeasureText(player1Text, fontSize);

    DrawText(
        player1Text,
        (int)centerX - scoreGap - player1TextWidth,
        20,
        fontSize,
        WHITE);

    DrawText(
        TextFormat("%d", player2Score),
        (int)centerX + scoreGap,
        20,
        fontSize,
        WHITE);

    // Draw countdown
    if (!gameOver && ballTimer > 0.0f)
    {
        int secondsRemaining = (int)ceilf(ballTimer);

        const char *timerText =
            TextFormat("%d", secondsRemaining);

        int timerWidth =
            MeasureText(timerText, largeFontSize);

        DrawText(
            timerText,
            GetScreenWidth() / 2 - timerWidth / 2,
            GetScreenHeight() / 2 - largeFontSize / 2,
            largeFontSize,
            GREEN);
    }

    // Draw winner
    if (gameOver && gameWinningText != NULL)
    {
        int winningTextWidth =
            MeasureText(gameWinningText, largeFontSize);

        DrawText(
            gameWinningText,
            GetScreenWidth() / 2 - winningTextWidth / 2,
            GetScreenHeight() / 2 - largeFontSize / 2,
            largeFontSize,
            GREEN);
    }
}

void UnloadGame(void)
{
}
