#ifndef POSITIONS_H
#define POSITIONS_H

#include <stdint.h>

static const uint32_t PositionsI[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0}};

static const uint32_t PositionsJ[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 1},
    {0, 0, 0, 0}};

static const uint32_t PositionsL[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 1},
    {0, 1, 1, 1},
    {0, 0, 0, 0}};

static const uint32_t PositionsO[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}};

static const uint32_t PositionsS[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 1, 1},
    {0, 1, 1, 0},
    {0, 0, 0, 0}};

static const uint32_t PositionsT[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 1, 0},
    {0, 1, 1, 1},
    {0, 0, 0, 0}};

static const uint32_t PositionsZ[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 0}};

#endif