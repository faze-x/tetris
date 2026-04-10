#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "input.h"
#include "tetromino.h"
#include "playfield.h"
#include "render.h"
#include "audio.h"
#include "gamestate.h"

static float lockTimer = 0;
InputState input;

void PollInput()
{
    input.leftHeld = IsKeyDown(KEY_LEFT);
    input.rightHeld = IsKeyDown(KEY_RIGHT);
    input.leftPressed = IsKeyPressed(KEY_LEFT);
    input.rightPressed = IsKeyPressed(KEY_RIGHT);

    if (input.leftHeld && !input.rightHeld)
        input.moveDir = -1;
    else if (input.rightHeld && !input.leftHeld)
        input.moveDir = 1;
    else
        input.moveDir = 0;

    input.downHeld = IsKeyDown(KEY_DOWN);

    input.rotatePressed = IsKeyPressed(KEY_UP);
    input.hardDropPressed = IsKeyPressed(KEY_SPACE);

}

void HandleHardDrop()
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

void HandleMovement(float dt)
{
    // tap movement
    if (input.leftPressed)
    {
        TryMove(-1);
        input.dasTimer = 0;
        input.arrTimer = 0;
    }

    if (input.rightPressed)
    {
        TryMove(1);
        input.dasTimer = 0;
        input.arrTimer = 0;
    }

    // hold movement
    if (input.moveDir != 0)
    {
        input.dasTimer += dt;

        if (input.dasTimer >= DAS_DELAY)
        {
            input.arrTimer += dt;

            while (input.arrTimer >= ARR_DELAY)
            {
                TryMove(input.moveDir);
                input.arrTimer -= ARR_DELAY;
            }
        }
    }
    else
    {
        input.dasTimer = 0;
        input.arrTimer = 0;
    }
}

void HandleSoftDrop()
{
    if (input.downHeld)
    {
        active.dropSpeed = 0.05f;
        if (active.fallTime > active.dropSpeed)
            active.fallTime = active.dropSpeed; // prevent accumulated time from causing a jump
    }
    else
    {
        active.dropSpeed = DROP_SPEED;
    }
}

void ApplyGravity()
{
    while (active.fallTime >= active.dropSpeed)
    {
        bool floorCollision = CheckCollisionAt(active.positionInGrid.x, active.positionInGrid.y + 1, active.tetro.cellPositions);
        if (!floorCollision)
        {
            active.positionInGrid.y += 1; // fall by 1 row
        }
        active.fallTime -= active.dropSpeed;
    }
}

void HandleLockDelay(float dt)
{
    bool grounded = CheckCollisionAt(
        active.positionInGrid.x,
        active.positionInGrid.y + 1,
        active.tetro.cellPositions
    );

    if (grounded)
    {
        lockTimer += dt;

        if (lockTimer >= (LOCK_DELAY_FRAMES / 60.0f))
        {
            LockActivePiece();
            SpawnNewPiece();
            lockTimer = 0;
        }
    }
    else
    {
        lockTimer = 0;
    }
}

void UpdateActiveTetromino()
{
    float dt = GetFrameTime();
    active.fallTime += GetFrameTime();
    active.rotateTime += GetFrameTime();
    //     active.moveTime += GetFrameTime();
    PollInput();

    if (input.hardDropPressed) HandleHardDrop();
    if (input.rotatePressed && active.rotateTime > 0.2) TryRotate();

    HandleMovement(dt);

    HandleSoftDrop();

    ApplyGravity();

    HandleLockDelay(dt);
}


void TryRotate()
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

void TryMove(int moveDir)
{
    bool wallCollision = CheckCollisionAt(active.positionInGrid.x + moveDir, active.positionInGrid.y, active.tetro.cellPositions);
    if (!wallCollision)
    {
        active.positionInGrid.x += moveDir; // move by 1 cell

        lockTimer = 0;
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