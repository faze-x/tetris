#include <stdlib.h>
#include <string.h>
#include "tetromino.h"
#include "positions.h"
#include "render.h"

Tetromino tetrominos[7];
TetrominoType pieceQueue[PIECE_QUEUE_SIZE];
int queueHead = 0;
int queueTail = 0;
ActiveTetromino active;

Tetromino MakeTetromino(TetrominoType type, const uint32_t src[4][4])
{

    Tetromino t = { .type = type };
    memcpy(t.cellPositions, src, sizeof t.cellPositions);
    switch (type)
    {
    case TYPE_I:
        t.blockTexture = textureBlockSkyblue;
        break;
    case TYPE_J:
        t.blockTexture = textureBlockBlue;
        break;
    case TYPE_L:
        t.blockTexture = textureBlockOrange;
        break;
    case TYPE_O:
        t.blockTexture = textureBlockYellow;
        break;
    case TYPE_S:
        t.blockTexture = textureBlockGreen;
        break;
    case TYPE_T:
        t.blockTexture = textureBlockPurple;
        break;
    case TYPE_Z:
        t.blockTexture = textureBlockRed;
        break;
    default:
        exit(-1); // something went terribly wrong, just quit atp;
        break;
    }
    return t;
}

void InitTetrominos()
{
    tetrominos[TYPE_I] = MakeTetromino(TYPE_I, PositionsI);
    tetrominos[TYPE_J] = MakeTetromino(TYPE_J, PositionsJ);
    tetrominos[TYPE_L] = MakeTetromino(TYPE_L, PositionsL);
    tetrominos[TYPE_O] = MakeTetromino(TYPE_O, PositionsO);
    tetrominos[TYPE_S] = MakeTetromino(TYPE_S, PositionsS);
    tetrominos[TYPE_T] = MakeTetromino(TYPE_T, PositionsT);
    tetrominos[TYPE_Z] = MakeTetromino(TYPE_Z, PositionsZ);
}

void ShuffleBag(TetrominoType* bag)
{
    for (int i = 6; i > 0; i--)
    {
        int j = rand() % (i + 1);

        TetrominoType temp = bag[i];
        bag[i] = bag[j];
        bag[j] = temp;
    }
}

void AddBagToQueue()
{
    // fill two shuffled bags
    for (int b = 0; b < 2; b++)
    {
        TetrominoType bag[7] = { TYPE_I, TYPE_J, TYPE_L, TYPE_O, TYPE_S, TYPE_T, TYPE_Z };
        ShuffleBag(bag);
        for (int i = 0; i < 7; i++)
        {
            pieceQueue[queueTail % PIECE_QUEUE_SIZE] = bag[i];
            queueTail++;
        }
    }
}

TetrominoType PopQueue()
{
    // refill when fewer than 7 pieces remain
    int remaining = queueTail - queueHead;
    if (remaining < 7)
        AddBagToQueue();

    return pieceQueue[queueHead++ % PIECE_QUEUE_SIZE];
}