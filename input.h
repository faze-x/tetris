#ifndef INPUT_H
#define INPUT_H
#include <stdint.h>

// static int lockFrames;

void UpdateActiveTetromino();
void TryRotateActive();
void RotateTetroPositions(uint32_t rotatedPositions[4][4], const uint32_t tetroPositions[4][4]);

#endif