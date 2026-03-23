#ifndef TETROMINO_H
#define TETROMINO_H

#include <raylib.h>
#include <stdint.h>
#include "constants.h"

typedef enum
{
    TYPE_I,
    TYPE_J,
    TYPE_L,
    TYPE_O,
    TYPE_S,
    TYPE_T,
    TYPE_Z
} TetrominoType;

typedef struct
{
    Texture2D blockTexture;
    uint32_t cellPositions[4][4];
    TetrominoType type;
} Tetromino;

typedef struct
{
    Tetromino tetro;

    struct
    {
        int x;
        int y;
    } positionInGrid;

    float fallTime;
    float rotateTime;
    float moveTime;
    float dropSpeed;
} ActiveTetromino;

extern Tetromino tetrominos[7];
extern TetrominoType pieceQueue[PIECE_QUEUE_SIZE];
extern int queueHead;
extern int queueTail;
extern ActiveTetromino active;

Tetromino MakeTetromino(TetrominoType type, const uint32_t src[4][4]);
void InitTetrominos();

void ShuffleBag(TetrominoType *bag);
void AddBagToQueue();
TetrominoType PopQueue();

#endif