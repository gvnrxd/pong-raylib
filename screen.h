#ifndef SCREEN_H
#define SCREEN_H

typedef enum GameScreen
{
    SCREEN_TITLE,
    SCREEN_GAME,
    SCREEN_SETTINGS
} GameScreen;

extern GameScreen currentScreen;

#endif