#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stddef.h>
#include "game.h"
#include "screen.h"

#define MAX_BOUNCE_ANGLE (75.0f * DEG2RAD)

static Rectangle bar2Rectangle;
static Rectangle bar1Rectangle;
static Rectangle menuBtn;
static Rectangle GrowRectangle(Rectangle rectangle, float amount)
{
    return (Rectangle){
        rectangle.x - amount / 2.0f,
        rectangle.y - amount / 2.0f,
        rectangle.width + amount,
        rectangle.height + amount};
}

static const int borderThickness = 5;
static const int centerLineThickness = 5;
static const int edgeSpacing = 30;
static const int paddleThickness = 10;
static const int paddleHeight = 120;
static const int winningScore = 2;

static const float roundCountdown = 3.0f;
static const float gameCountdown = 3.0f;
static const float hoverGrowth = 10.0f;

static int player1Score;
static int player2Score;

static float previousBallVelocityX = 0.0f;
static float previousBallVelocityY = 0.0f;
static float ballTimer;
static float defaultVelocity = 900.0f;
static float ballVelocityX = 900.0f;
static float ballVelocityY = 0.0f;

static const float paddleMoveSpeed = 18.0f;
static const float ballRadius = 20.0f;

static bool gameOver;
static bool gamePaused;
static bool menuBtnHovered;

static const char *gameWinningText;

static Vector2 bar2Position;
static Vector2 bar1Position;
static Vector2 ballPosition;

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

void RestartGame(void)
{
    gameOver = false;
    gamePaused = false;

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
    gamePaused = false;

    menuBtn = (Rectangle){
        20,
        100,
        150,
        50};
}

void UpdateGame(void)
{
    const KeyboardKey bar1UpKey = KEY_W;
    const KeyboardKey bar1DownKey = KEY_S;

    if (gamePaused)
    {
        ShowCursor();
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

        Vector2 cursorPosition = GetMousePosition();

        menuBtnHovered = CheckCollisionPointRec(cursorPosition, menuBtn);

        // Change mouse state
        if (menuBtnHovered)
        {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        }
        else
        {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        // Handle changing screen state
        if (menuBtnHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            RestartGame();
            currentScreen = SCREEN_TITLE;
        }
    }

    if (!gamePaused)
    {
        HideCursor();
    }

    // Go back to home screen
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        gamePaused = false;
        currentScreen = SCREEN_TITLE;
    }

    // Pause game

    if (IsKeyPressed(KEY_P))
    {
        gamePaused = !gamePaused;

        if (gamePaused)
        {
            ShowCursor();

            previousBallVelocityX = ballVelocityX;
            previousBallVelocityY = ballVelocityY;

            ballVelocityX = 0.0f;
            ballVelocityY = 0.0f;
        }
        else
        {
            ballVelocityX = previousBallVelocityX;
            ballVelocityY = previousBallVelocityY;
        }
    }

    // Restart game
    if (IsKeyPressed(KEY_R) || IsKeyPressed(KEY_BACKSPACE))
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
    if (!gameOver && !gamePaused)
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
        if (IsKeyDown(bar1UpKey) && !gamePaused)
        {
            bar1Position.y -= paddleMoveSpeed;
        }

        if (IsKeyDown(bar1DownKey) && !gamePaused)
        {
            bar1Position.y += paddleMoveSpeed;
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
    if (!gameOver && ballTimer > 0.0f && !gamePaused)
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

    // Draw Pause Screen
    if (gamePaused)
    {
        const char *gamePausedText = "Paused";

        int gamePausedTextWidth = MeasureText(gamePausedText, largeFontSize);

        DrawText(
            gamePausedText,
            GetScreenWidth() / 2 - gamePausedTextWidth / 2,
            largeFontSize * 2,
            largeFontSize,
            GREEN);

        Rectangle drawnMenuBtn = menuBtnHovered ? GrowRectangle(menuBtn, hoverGrowth) : menuBtn;
        DrawRectangleRec(drawnMenuBtn, WHITE);

        const char *menuBtnText = "<- Back to Menu";
        const int buttonFontSize = 20;
        const int menuBtnTextWidth = MeasureText(menuBtnText, buttonFontSize);

        DrawText(
            menuBtnText,
            drawnMenuBtn.x + (drawnMenuBtn.width - menuBtnTextWidth) / 2,
            drawnMenuBtn.y + (drawnMenuBtn.height - buttonFontSize) / 2,
            buttonFontSize,
            BLACK);
    }
}

void UnloadGame(void)
{
}
