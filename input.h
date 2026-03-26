#ifndef INPUT_H
#define INPUT_H
#include <stdint.h>
#include <stdbool.h>

// static int lockFrames;

typedef struct
{
    // raw inputs
    bool leftHeld;
    bool rightHeld;

    bool leftPressed;
    bool rightPressed;

    // DAS/ARR
    float dasTimer;
    float arrTimer;

    int moveDir; // -1, 0, +1

    // soft drop
    bool downHeld;

    bool rotatePressed;
    bool hardDropPressed;

} InputState;

void PollInput();
void HandleHardDrop();
void HandleMovement(float dt);
void HandleSoftDrop();
void ApplyGravity();
void HandleLockDelay(float dt);

void UpdateActiveTetromino();
void TryRotate();
void TryMove();
void RotateTetroPositions(uint32_t rotatedPositions[4][4], const uint32_t tetroPositions[4][4]);

#endif