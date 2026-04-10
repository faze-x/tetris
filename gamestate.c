#include <raylib.h>
#include "gamestate.h"
#include "playfield.h"

GameState currentGameState = STATE_PLAYING;

void StartGame()
{
    currentGameState = STATE_PLAYING;
    InitPlayfield();
    SpawnNewPiece();
}

void HandleGameOver()
{
    if (IsKeyPressed(KEY_R)) {
        StartGame();
    }
}