#include "raylib.h"
#include "settings.h"
#include "screen.h"

// Button layout constants
static const float buttonWidth = 150.0f;
static const float buttonHeight = 50.0f;
static const float hoverGrowth = 10.0f;

// Button bounds
static Rectangle backButton;

// Button states
static bool backButtonHovered;

static Rectangle GrowRectangle(Rectangle rectangle, float amount)
{
    return (Rectangle){
        rectangle.x - amount / 2.0f,
        rectangle.y - amount / 2.0f,
        rectangle.width + amount,
        rectangle.height + amount};
}

void InitSettings(void)
{
    backButton = (Rectangle){
        20,
        100,
        buttonWidth,
        buttonHeight,
    };
}

void UpdateSettings(void)
{
    Vector2 mousePosition = GetMousePosition();

    backButtonHovered = CheckCollisionPointRec(mousePosition, backButton);

    // Change mouse state
    if (backButtonHovered)
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    // Handle changing screen state
    if (backButtonHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        currentScreen = SCREEN_TITLE;
    }
}

void DrawSettings(void)
{
    ClearBackground(BLACK);
    // Title
    const char *title = "Settings";
    const int titleFontSize = 40;
    const int titleWidth = MeasureText(title, titleFontSize);

    DrawText(
        title,
        (GetScreenWidth() - titleWidth) / 2,
        GetScreenHeight() / 2 - 50,
        titleFontSize,
        WHITE);

    Rectangle drawnBackButton = backButtonHovered ? GrowRectangle(backButton, hoverGrowth) : backButton;
    DrawRectangleRec(drawnBackButton, WHITE);

    const char *backText = "<- Go Back";
    const int buttonFontSize = 20;
    const int backTextWidth = MeasureText(backText, buttonFontSize);

    DrawText(
        backText,
        drawnBackButton.x +
            (drawnBackButton.width - backTextWidth) / 2,
        drawnBackButton.y +
            (drawnBackButton.height - buttonFontSize) / 2,
        buttonFontSize,
        BLACK);
}

void UnloadSettings(void) {}
