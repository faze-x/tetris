#ifndef GAMESTATE_H
#define GAMESTATE_H

typedef enum {
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_MENU,
    STATE_OVER
} GameState;

extern GameState currentGameState;

void StartGame();
void HandleGameOver();

#endif