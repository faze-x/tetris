#ifndef CONSTANTS_H
#define CONSTANTS_H

// #define WINDOW_WIDTH 900
// #define WINDOW_HEIGHT 506
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define PLAYFIELD_WIDTH 10
#define PLAYFIELD_HEIGHT 20
// #define PLAYFIELD_BORDER_THICKNESS 2.0f // pixels
// #define CELL_SIZE (WINDOW_WIDTH / 45)
#define CELL_SIZE (WINDOW_HEIGHT / (PLAYFIELD_HEIGHT + 4))
#define FONT_SIZE (CELL_SIZE * 1.5f)

#define DROP_SPEED 0.6f
#define DROP_SPEED_DECREMENT 0.01f
#define DROP_SPEED_MINIMUM 0.1f

#define LOCK_DELAY_FRAMES 15
#define PIECE_QUEUE_SIZE 14

#define PLAYFIELD_OFFSET_LEFT ((WINDOW_WIDTH / 2) - ((PLAYFIELD_WIDTH / 2) * CELL_SIZE))
#define PLAYFIELD_OFFSET_UP (WINDOW_HEIGHT - (1 + PLAYFIELD_HEIGHT) * CELL_SIZE)

#define PLAYFIELD_TEXTURE_WIDTH 200
#define PLAYFIELD_TEXTURE_HEIGHT 400
#define PLAYFIELD_TEXTURE_BORDER 5 // px border in the original texture

// derived: how many pixels the border takes up when texture is scaled to fit the grid
#define PLAYFIELD_GRID_PIXEL_WIDTH (PLAYFIELD_WIDTH * CELL_SIZE)
#define PLAYFIELD_GRID_PIXEL_HEIGHT (PLAYFIELD_HEIGHT * CELL_SIZE)

#define PLAYFIELD_BORDER_SCALED_X ((float)PLAYFIELD_TEXTURE_BORDER / PLAYFIELD_TEXTURE_WIDTH * PLAYFIELD_GRID_PIXEL_WIDTH)
#define PLAYFIELD_BORDER_SCALED_Y ((float)PLAYFIELD_TEXTURE_BORDER / PLAYFIELD_TEXTURE_HEIGHT * PLAYFIELD_GRID_PIXEL_HEIGHT)

#define BG_SCALE ((float)WINDOW_WIDTH / 576.0f)
#define SCREEN_SHAKE_INTENSITY 4.0f

static const uint32_t wallKicks[][2] = {
    {0, 0},  // no kick
    {-1, 0}, // left
    {1, 0},  // right
    {-2, 0}, // bigger left
    {2, 0},  // bigger right
    {0, -1}  // up (floor kick)
};

#endif