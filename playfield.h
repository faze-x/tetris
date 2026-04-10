#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include <raylib.h>
#include <stdint.h>
#include "constants.h"

typedef struct
{
    Texture2D blockTexture;
    bool occupied;
} PlayfieldCell;

extern PlayfieldCell playfieldGrid[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH];

void InitPlayfield();
bool CheckCollisionAt(int x, int y, const uint32_t tetroPositions[4][4]); // grid position x y
void HandleLineClears();
void LockActivePiece();
void SpawnNewPiece();

#endif