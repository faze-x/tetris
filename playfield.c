#include <stdio.h>
#include "playfield.h"
#include "tetromino.h"

PlayfieldCell playfieldGrid[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH];
bool gameOver = false;

void InitPlayfield()
{
    for (int i = 0; i < PLAYFIELD_HEIGHT; i++)
    {
        for (int j = 0; j < PLAYFIELD_WIDTH; j++)
        {
            playfieldGrid[i][j].occupied = false;
        }
    }
}

bool CheckCollisionAt(int x, int y, const uint32_t matrix[4][4])
{
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            if (matrix[j][i] == 0)
                continue;

            int bx = x + i;
            int by = y + j;

            // left / right walls
            if (bx < 0 || bx >= PLAYFIELD_WIDTH)
                return true;

            // floor
            if (by >= PLAYFIELD_HEIGHT)
                return true;

            // ceiling allowed (spawn)
            if (by < 0)
                continue;

            if (playfieldGrid[by][bx].occupied)
                return true;
        }
    }
    return false;
}

void HandleLineClears()
{
    for (int y = 0; y < PLAYFIELD_HEIGHT; y++)
    {
        bool full = true;

        for (int x = 0; x < PLAYFIELD_WIDTH; x++)
        {
            if (!playfieldGrid[y][x].occupied)
            {
                full = false;
                break;
            }
        }

        if (full)
        {
            for (int row = y; row > 0; row--)
            {
                for (int x = 0; x < PLAYFIELD_WIDTH; x++)
                {
                    playfieldGrid[row][x] = playfieldGrid[row - 1][x];
                }
            }

            y++; // re-check same row after collapse
        }
    }
}

void LockActivePiece()
{
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            if (active.tetro.cellPositions[j][i] == 1)
            {
                int piecePosX = active.positionInGrid.x + i;
                int piecePosY = active.positionInGrid.y + j;

                if (piecePosY >= 0)
                {
                    playfieldGrid[piecePosY][piecePosX].occupied = true;
                    playfieldGrid[piecePosY][piecePosX].blockTexture = active.tetro.blockTexture;
                }
            }
        }
    }
}

void SpawnNewPiece()
{
    active.fallTime = 0;
    active.rotateTime = 0;
    active.dropSpeed = DROP_SPEED;

    active.positionInGrid.x = PLAYFIELD_WIDTH / 2 - 2; // centered
    active.positionInGrid.y = -4;                      // above playfield
    
    TetrominoType type = PopQueue();
    
    active.tetro = tetrominos[type];

    if (CheckCollisionAt(active.positionInGrid.x, active.positionInGrid.y + 2, active.tetro.cellPositions))
    {
        gameOver = true;
    }
}
    