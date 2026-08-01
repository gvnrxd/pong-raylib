#include "raylib.h"
#include "title_screen.h"
#include "screen.h"

// Button layout constants
static const float buttonWidth = 150.0f;
static const float buttonHeight = 50.0f;
static const float buttonGap = 20.0f;
static const float hoverGrowth = 10.0f;

// Button bounds
static Rectangle startButton;
static Rectangle settingsButton;

// Button states
static bool startButtonHovered;
static bool settingsButtonHovered;

static Rectangle GrowRectangle(Rectangle rectangle, float amount)
{
    return (Rectangle){
        rectangle.x - amount / 2.0f,
        rectangle.y - amount / 2.0f,
        rectangle.width + amount,
        rectangle.height + amount};
}

void InitTitleScreen(void)
{
    const float groupWidth = buttonWidth * 2.0f + buttonGap;
    const float startX =
        (GetScreenWidth() - groupWidth) / 2.0f;
    const float buttonY =
        GetScreenHeight() / 2.0f + 50.0f;

    startButton = (Rectangle){
        startX,
        buttonY,
        buttonWidth,
        buttonHeight};

    settingsButton = (Rectangle){
        startX + buttonWidth + buttonGap,
        buttonY,
        buttonWidth,
        buttonHeight};
}

void UpdateTitleScreen(void)
{
    Vector2 mousePosition = GetMousePosition();

    // Change mouse states
    startButtonHovered =
        CheckCollisionPointRec(mousePosition, startButton);

    settingsButtonHovered =
        CheckCollisionPointRec(mousePosition, settingsButton);

    if (startButtonHovered || settingsButtonHovered)
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    // Handle changing screen state

    if (startButtonHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        currentScreen = SCREEN_GAME;
    }

    if (settingsButtonHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        currentScreen = SCREEN_SETTINGS;
    }
}

void DrawTitleScreen(void)
{
    // Title
    const char *title = "Welcome to Pong";
    const int titleFontSize = 40;
    const int titleWidth = MeasureText(title, titleFontSize);

    DrawText(
        title,
        (GetScreenWidth() - titleWidth) / 2,
        GetScreenHeight() / 2 - 50,
        titleFontSize,
        BLACK);

    // Create the rectangles that will actually be drawn
    Rectangle drawnStartButton = startButtonHovered
                                     ? GrowRectangle(startButton, hoverGrowth)
                                     : startButton;

    Rectangle drawnSettingsButton = settingsButtonHovered
                                        ? GrowRectangle(settingsButton, hoverGrowth)
                                        : settingsButton;

    // Draw buttons
    DrawRectangleRec(drawnStartButton, RED);
    DrawRectangleRec(drawnSettingsButton, BLACK);

    // Button text
    const char *startText = "Start Game";
    const char *settingsText = "Settings";
    const int buttonFontSize = 20;

    const int startTextWidth =
        MeasureText(startText, buttonFontSize);

    const int settingsTextWidth =
        MeasureText(settingsText, buttonFontSize);

    DrawText(
        startText,
        drawnStartButton.x +
            (drawnStartButton.width - startTextWidth) / 2,
        drawnStartButton.y +
            (drawnStartButton.height - buttonFontSize) / 2,
        buttonFontSize,
        WHITE);

    DrawText(
        settingsText,
        drawnSettingsButton.x +
            (drawnSettingsButton.width - settingsTextWidth) / 2,
        drawnSettingsButton.y +
            (drawnSettingsButton.height - buttonFontSize) / 2,
        buttonFontSize,
        WHITE);
}