#ifndef DATA_H
#define DATA_H

#include "Block.h"

#define NUM_BLOCK_COLORS 7
#define NUM_BLOCK_TYPES 7
#define BLOCK_LAYOUT_SIZE 16

enum BlockTypes {
    I_PIECE = 0,
    J_PIECE,
    L_PIECE,
    O_PIECE,
    S_PIECE,
    T_PIECE,
    Z_PIECE,
};

static const int BLOCK_LAYOUTS[7][16] = {
    {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, // I
    {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // J
    {0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // L
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0}, // O
    {0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // S
    {0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // T
    {1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Z
};

static const float BLOCK_ORIGINS[7][2] = {
    {2.0f, 2.0f}, // I
    {1.5f, 1.5f}, // J
    {1.5f, 1.5f}, // L
    {2.0f, 2.0f}, // O
    {1.5f, 1.5f}, // S
    {1.5f, 1.5f}, // T
    {1.5f, 1.5f}, // Z
};

// The following are offsets to be checked for wall kicks

typedef struct {
    int x;
    int y;
} Offset;

// J, L, S, T, Z Tetromino Offset Data
static const Offset OFFSETS[4][5] = {
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}};

// I Tetromino Offset Data
static const Offset I_PIECE_OFFSETS[4][5] = {
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}},
    {{0, 0}, {3, 0}, {-3, 0}, {3, -1}, {-3, -1}},
    {{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}}};

#endif