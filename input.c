#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "input.h"
#include "tetromino.h"
#include "playfield.h"
#include "render.h"
#include "audio.h"

static int lockFrames = 0;

void UpdateActiveTetromino()
{
    active.fallTime += GetFrameTime();
    active.rotateTime += GetFrameTime();
    active.moveTime += GetFrameTime();

    // rotation
    bool rotate = false;
    if (IsKeyDown(KEY_UP))
        rotate = true;

    if (rotate && active.rotateTime >= 0.2)
    {
        TryRotateActive();
        rotate = false;
        active.rotateTime = 0;

        lockFrames = 0;
    }

    // left right movement
    int move = 0;
    if (IsKeyDown(KEY_LEFT))
        move = -1;
    if (IsKeyDown(KEY_RIGHT))
        move = 1;

    bool wallCollision = CheckCollisionAt(active.positionInGrid.x + move, active.positionInGrid.y, active.tetro.cellPositions);
    if (abs(move) == 1 && !wallCollision && active.moveTime >= 0.15)
    {
        active.positionInGrid.x += move; // move by 1 cell
        active.moveTime = 0;

        lockFrames = 0;
    }

    // soft drop (no cooldown cause i cant bother)
    if (IsKeyReleased(KEY_DOWN) && active.dropSpeed >= DROP_SPEED_MINIMUM)
        active.dropSpeed -= DROP_SPEED_DECREMENT;
    // printf("%f\n", active.dropSpeed);

    // hard drop
    if (IsKeyReleased(KEY_SPACE))
    {
        while (!CheckCollisionAt(
            active.positionInGrid.x,
            active.positionInGrid.y + 1,
            active.tetro.cellPositions))
        {
            active.positionInGrid.y++;
        }
        // screen shake
        shakeTimer = 0.2f;
        SetSoundPitch(hardDropSFX, ((float)rand() / RAND_MAX) * 0.2 + 0.9);
        PlaySound(hardDropSFX);
    }

    // gravity
    bool floorCollision = CheckCollisionAt(active.positionInGrid.x, active.positionInGrid.y + 1, active.tetro.cellPositions);
    if (active.fallTime >= active.dropSpeed)
    {
        if (!floorCollision)
        {
            active.positionInGrid.y += 1; // fall by 1 row
        }
        active.fallTime = 0;
    }

    // lock delay
    bool grounded = CheckCollisionAt(
        active.positionInGrid.x,
        active.positionInGrid.y + 1,
        active.tetro.cellPositions);

    if (grounded)
    {
        lockFrames++;

        if (lockFrames >= LOCK_DELAY_FRAMES)
        {
            LockActivePiece();
            SpawnNewPiece();
            lockFrames = 0;
        }
    }
    else
    {
        lockFrames = 0;
    }
}

void TryRotateActive()
{
    // check rotate collisions
    uint32_t rotatedPositions[4][4];
    RotateTetroPositions(rotatedPositions, active.tetro.cellPositions);

    for (int i = 0; i < 6; i++) // UPDATE ACCORDING TO NUM OF WALLKICKS!!!
    {
        int dx = active.positionInGrid.x + wallKicks[i][0];
        int dy = active.positionInGrid.y + wallKicks[i][1];

        if (!CheckCollisionAt(dx, dy, rotatedPositions))
        {
            active.positionInGrid.x = dx;
            active.positionInGrid.y = dy;
            memcpy(active.tetro.cellPositions, rotatedPositions, sizeof(rotatedPositions));
            return;
        }
    }
}

void RotateTetroPositions(uint32_t rotatedPositions[4][4], const uint32_t tetroPositions[4][4])
{
    memcpy(rotatedPositions, tetroPositions, 4 * 4 * sizeof(uint32_t));

    int temp;

    temp = rotatedPositions[0][1];
    rotatedPositions[0][1] = rotatedPositions[1][0];
    rotatedPositions[1][0] = temp;

    temp = rotatedPositions[0][2];
    rotatedPositions[0][2] = rotatedPositions[2][0];
    rotatedPositions[2][0] = temp;

    temp = rotatedPositions[0][3];
    rotatedPositions[0][3] = rotatedPositions[3][0];
    rotatedPositions[3][0] = temp;

    temp = rotatedPositions[1][2];
    rotatedPositions[1][2] = rotatedPositions[2][1];
    rotatedPositions[2][1] = temp;

    temp = rotatedPositions[1][3];
    rotatedPositions[1][3] = rotatedPositions[3][1];
    rotatedPositions[3][1] = temp;

    temp = rotatedPositions[2][3];
    rotatedPositions[2][3] = rotatedPositions[3][2];
    rotatedPositions[3][2] = temp;

    for (int i = 0; i < 4; i++)
    {
        temp = rotatedPositions[i][0];
        rotatedPositions[i][0] = rotatedPositions[i][3];
        rotatedPositions[i][3] = temp;

        temp = rotatedPositions[i][1];
        rotatedPositions[i][1] = rotatedPositions[i][2];
        rotatedPositions[i][2] = temp;
    }
}