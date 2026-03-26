#ifndef CONSTANTS_H
#define CONSTANTS_H

#define FRAME_RATE 60

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// playfield grid size
#define PLAYFIELD_WIDTH 10
#define PLAYFIELD_HEIGHT 20

// cell size should be a const int later
#define CELL_SIZE (WINDOW_HEIGHT / (PLAYFIELD_HEIGHT + 4))
#define FONT_SIZE (CELL_SIZE * 1.5f)

// ------------------ movement ------------------
// after how many ms the tetro moves 1 block down
#define DROP_SPEED 0.6f
#define DAS_DELAY 0.15f   // delay before repeat starts (in s)
#define ARR_DELAY 0.05f   // repeat speed (in s)
#define LOCK_DELAY_FRAMES 15
// ----------------------------------------------

// ------------------ spawn bag ------------------
#define PIECE_QUEUE_SIZE 14
// -----------------------------------------------

// to draw the playfield in the center
#define PLAYFIELD_OFFSET_LEFT ((WINDOW_WIDTH / 2) - ((PLAYFIELD_WIDTH / 2) * CELL_SIZE))
#define PLAYFIELD_OFFSET_UP (WINDOW_HEIGHT - (1 + PLAYFIELD_HEIGHT) * CELL_SIZE)

// ------------------ playfield texture ------------------
#define PLAYFIELD_TEXTURE_WIDTH 200
#define PLAYFIELD_TEXTURE_HEIGHT 400
#define PLAYFIELD_TEXTURE_BORDER 5 // px border in the original texture
// -------------------------------------------------------

#define PLAYFIELD_GRID_PIXEL_WIDTH (PLAYFIELD_WIDTH * CELL_SIZE)
#define PLAYFIELD_GRID_PIXEL_HEIGHT (PLAYFIELD_HEIGHT * CELL_SIZE)

// derived: how many pixels the border takes up when texture is scaled to fit the grid
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