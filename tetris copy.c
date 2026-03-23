// gcc -Wall -Wextra -o tetris tetris.c -lraylib -lopengl32 -lgdi32 -lwinmm -lm
// TODO
// 1. add lock delay - done
// 2. split the code into separate files ----
// 3. make it look prettier with pre drawn sprites (?)

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include "positions.h"
#include "constants.h"

#define BACKGROUND_FOLDER "textures/background/Clouds 4"

#define TEXTURE_BLOCK_SKYBLUE "textures/blocks/skyblue.png"
#define TEXTURE_BLOCK_BLUE "textures/blocks/blue.png"
#define TEXTURE_BLOCK_ORANGE "textures/blocks/orange.png"
#define TEXTURE_BLOCK_YELLOW "textures/blocks/yellow.png"
#define TEXTURE_BLOCK_GREEN "textures/blocks/green.png"
#define TEXTURE_BLOCK_PURPLE "textures/blocks/purple.png"
#define TEXTURE_BLOCK_RED "textures/blocks/red.png"
#define TEXTURE_PLAYFIELD "textures/playfield1.png"

// load block textures
Texture2D textureBlockSkyblue, textureBlockBlue, textureBlockOrange, textureBlockYellow, textureBlockGreen, textureBlockPurple, textureBlockRed;
Texture2D playfieldTexture;
// -------------------

#define PLAYFIELD_OFFSET_LEFT ((WINDOW_WIDTH / 2) - ((PLAYFIELD_WIDTH / 2) * CELL_SIZE))
#define PLAYFIELD_OFFSET_UP (WINDOW_HEIGHT - (1 + PLAYFIELD_HEIGHT) * CELL_SIZE)

#define PLAYFIELD_PIXEL_WIDTH (PLAYFIELD_WIDTH * CELL_SIZE)
#define TETRO_SPAWN_OFFSET_LEFT (PLAYFIELD_OFFSET_LEFT + (PLAYFIELD_PIXEL_WIDTH / 2) - (2 * CELL_SIZE))
#define TETRO_SPAWN_OFFSET_UP (PLAYFIELD_OFFSET_UP - (4 * CELL_SIZE))

static int lockFrames = 0;

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

static const uint32_t wallKicks[][2] = {
    {0, 0},  // no kick
    {-1, 0}, // left
    {1, 0},  // right
    {-2, 0}, // bigger left
    {2, 0},  // bigger right
    {0, -1}  // up (floor kick)
};

typedef struct
{
    Texture2D blockTexture;
    bool occupied;
} PlayfieldCell;

TetrominoType pieceQueue[PIECE_QUEUE_SIZE];
int queueHead = 0;
int queueTail = 0;

PlayfieldCell playfieldGrid[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH];

// tetro bag functions
Tetromino tetrominos[7];
void ShuffleBag(TetrominoType *bag);
void AddBagToQueue();
TetrominoType PopQueue();

Tetromino MakeTetromino(TetrominoType type, const uint32_t src[4][4]);
void InitTetrominos();
void LoadBackgroundTextures(Texture *textures, int *num_textures);
void LoadBlockTextures();

void DrawBackground(Texture *textures, int num_textures);
void DrawPlayfield();
void DrawActiveTetromino();
void DrawShadowPiece();

void InitPlayfield();
void HandleLineClears();
void UpdateActiveTetromino();
void TryRotateActive();
void RotateTetroPositions(uint32_t rotatedPositions[4][4], const uint32_t tetroPositions[4][4]);
bool CheckCollisionAt(int x, int y, const uint32_t tetroPositions[4][4]); // grid position x y
void LockActivePiece();
void SpawnNewPiece();

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

ActiveTetromino active;

int main(void)
{
    srand(time(NULL));

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetoris");
    LoadBlockTextures();

    // make tetromino variables
    InitTetrominos();
    InitPlayfield();
    playfieldTexture = LoadTexture(TEXTURE_PLAYFIELD);

    // active tetro
    SpawnNewPiece();

    int num_textures = 0;
    Texture2D bgTextures[100];
    LoadBackgroundTextures(bgTextures, &num_textures);

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        UpdateActiveTetromino();

        DrawBackground(bgTextures, num_textures);

        DrawPlayfield();
        DrawActiveTetromino();
        DrawShadowPiece();
        HandleLineClears();

        EndDrawing();
    }

    CloseWindow();
    return 0;
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

void LoadBackgroundTextures(Texture *textures, int *num_textures)
{
    DIR *dir = opendir(BACKGROUND_FOLDER);
    if (dir == NULL)
        return;

    struct dirent *entry;
    char full_path[PATH_MAX * 2 + 1];
    int i = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        // Skip "." and ".." entries to avoid issues and infinite loops in recursive scenarios
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // Use snprintf to safely combine the directory path and the filename
        snprintf(full_path, sizeof(full_path), "%s/%s", BACKGROUND_FOLDER, entry->d_name);

        textures[i++] = LoadTexture(full_path);
    }

    *num_textures = i;
    // Close the directory
    closedir(dir);
}

void LoadBlockTextures()
{
    textureBlockSkyblue = LoadTexture(TEXTURE_BLOCK_SKYBLUE);
    textureBlockBlue = LoadTexture(TEXTURE_BLOCK_BLUE);
    textureBlockOrange = LoadTexture(TEXTURE_BLOCK_ORANGE);
    textureBlockYellow = LoadTexture(TEXTURE_BLOCK_YELLOW);
    textureBlockGreen = LoadTexture(TEXTURE_BLOCK_GREEN);
    textureBlockPurple = LoadTexture(TEXTURE_BLOCK_PURPLE);
    textureBlockRed = LoadTexture(TEXTURE_BLOCK_RED);
}

void DrawBackground(Texture *textures, int num_textures)
{
    for (int i = 0; i < num_textures; i++)
    {
        Rectangle source = {0, 0, textures[i].width, textures[i].height};
        Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
        Vector2 origin = {0, 0};
        DrawTexturePro(textures[i], source, dest, origin, 0, (Color){255, 255, 255, 255});
    }
}

void DrawPlayfield()
{
    // Vector2 leftLineStartPos = {PLAYFIELD_OFFSET_LEFT, PLAYFIELD_OFFSET_UP};
    // Vector2 leftLineEndPos = {leftLineStartPos.x, leftLineStartPos.y + (PLAYFIELD_HEIGHT * CELL_SIZE)};
    // Vector2 downLineStartPos = {leftLineEndPos.x, leftLineEndPos.y};
    // Vector2 downLineEndPos = {downLineStartPos.x + (PLAYFIELD_WIDTH * CELL_SIZE), downLineStartPos.y};
    // Vector2 rightLineStartPos = {downLineEndPos.x, downLineEndPos.y};
    // Vector2 rightLineEndPos = {rightLineStartPos.x, rightLineStartPos.y - (PLAYFIELD_HEIGHT * CELL_SIZE)};

    // draw textured bg
    Rectangle source = {0, 0, playfieldTexture.width, playfieldTexture.height};
    Rectangle dest = {PLAYFIELD_OFFSET_LEFT - 5, PLAYFIELD_OFFSET_UP - 5, (float)(PLAYFIELD_WIDTH * CELL_SIZE + 10), (float)(PLAYFIELD_HEIGHT * CELL_SIZE + 10)};
    Vector2 origin = {0, 0};
    DrawTexturePro(playfieldTexture, source, dest, origin, 0, (Color){255, 255, 255, 255});
    // -------

    // draw existing blocks
    for (int i = 0; i < PLAYFIELD_HEIGHT; i++)
    {
        for (int j = 0; j < PLAYFIELD_WIDTH; j++)
        {
            if (playfieldGrid[i][j].occupied)
            {
                Vector2 occupiedPos = {
                    PLAYFIELD_OFFSET_LEFT + j * CELL_SIZE,
                    PLAYFIELD_OFFSET_UP + i * CELL_SIZE};

                Rectangle source = {0, 0, playfieldGrid[i][j].blockTexture.width, playfieldGrid[i][j].blockTexture.height};
                Rectangle dest = {occupiedPos.x, occupiedPos.y, (float)CELL_SIZE, (float)CELL_SIZE};
                Vector2 origin = {0, 0};
                DrawTexturePro(playfieldGrid[i][j].blockTexture, source, dest, origin, 0, (Color){255, 255, 255, 255});
            }
        }
    }

    // DrawLineEx(leftLineStartPos, leftLineEndPos, PLAYFIELD_BORDER_THICKNESS, RAYWHITE);
    // DrawLineEx(rightLineStartPos, rightLineEndPos, PLAYFIELD_BORDER_THICKNESS, RAYWHITE);
    // DrawLineEx(downLineStartPos, downLineEndPos, PLAYFIELD_BORDER_THICKNESS, RAYWHITE);
}

void InitPlayfield()
{
    for (int i = 0; i < PLAYFIELD_HEIGHT; i++)
    {
        for (int j = 0; j < PLAYFIELD_WIDTH; j++)
        {
            playfieldGrid[i][j].occupied = false;
        }
    }
}

Tetromino MakeTetromino(TetrominoType type, const uint32_t src[4][4])
{
    Tetromino t = {.type = type};
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

void ShuffleBag(TetrominoType *bag)
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
    TetrominoType bag[7] = {
        TYPE_I,
        TYPE_J,
        TYPE_L,
        TYPE_O,
        TYPE_S,
        TYPE_T,
        TYPE_Z};

    ShuffleBag(bag);

    for (int i = 0; i < 7; i++)
    {
        pieceQueue[queueTail++] = bag[i];
    }
}

TetrominoType PopQueue()
{
    if (queueHead >= queueTail)
        AddBagToQueue();

    return pieceQueue[queueHead++];
}

bool CheckCollisionAt(int x, int y, const uint32_t matrix[4][4])
{
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            if (matrix[j][i] == 0)
                continue;

            int bx = x + i;
            int by = y + j;

            // left / right walls
            if (bx < 0 || bx >= PLAYFIELD_WIDTH)
                return true;

            // floor
            if (by >= PLAYFIELD_HEIGHT)
                return true;

            // ceiling allowed (spawn)
            if (by < 0)
                continue;

            if (playfieldGrid[by][bx].occupied)
                return true;
        }
    }
    return false;
}

void HandleLineClears()
{
    for (int y = 0; y < PLAYFIELD_HEIGHT; y++)
    {
        bool full = true;

        for (int x = 0; x < PLAYFIELD_WIDTH; x++)
        {
            if (!playfieldGrid[y][x].occupied)
            {
                full = false;
                break;
            }
        }

        if (full)
        {
            for (int row = y; row > 0; row--)
            {
                for (int x = 0; x < PLAYFIELD_WIDTH; x++)
                {
                    playfieldGrid[row][x] = playfieldGrid[row - 1][x];
                }
            }

            y++; // re-check same row after collapse
        }
    }
}

void LockActivePiece()
{
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            if (active.tetro.cellPositions[j][i] == 1)
            {
                int piecePosX = active.positionInGrid.x + i;
                int piecePosY = active.positionInGrid.y + j;

                if (piecePosY >= 0)
                {
                    playfieldGrid[piecePosY][piecePosX].occupied = true;
                    playfieldGrid[piecePosY][piecePosX].blockTexture = active.tetro.blockTexture;
                }
            }
        }
    }
}

void SpawnNewPiece()
{
    active.fallTime = 0;
    active.rotateTime = 0;
    active.moveTime = 0;
    active.dropSpeed = DROP_SPEED;

    active.positionInGrid.x = PLAYFIELD_WIDTH / 2 - 2; // centered
    active.positionInGrid.y = -2;                      // above playfield

    TetrominoType type = PopQueue();

    active.tetro = tetrominos[type];
}

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
    if (IsKeyDown(KEY_DOWN) && active.dropSpeed >= DROP_SPEED_MINIMUM)
        active.dropSpeed -= DROP_SPEED_DECREMENT;

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

void DrawActiveTetromino()
{
    Vector2 currentPos = {
        PLAYFIELD_OFFSET_LEFT + active.positionInGrid.x * CELL_SIZE,
        PLAYFIELD_OFFSET_UP + active.positionInGrid.y * CELL_SIZE};

    for (int i = 0; i < 4; i++)
    {
        currentPos.x = PLAYFIELD_OFFSET_LEFT + active.positionInGrid.x * CELL_SIZE;
        for (int j = 0; j < 4; j++)
        {
            if (active.tetro.cellPositions[i][j] == 1)
            {
                Rectangle source = {0, 0, active.tetro.blockTexture.width, active.tetro.blockTexture.height};
                Rectangle dest = {currentPos.x, currentPos.y, (float)CELL_SIZE, (float)CELL_SIZE};
                Vector2 origin = {0, 0};
                DrawTexturePro(active.tetro.blockTexture, source, dest, origin, 0, (Color){255, 255, 255, 255});

                // Rectangle rec = {.x = currentPos.x, .y = currentPos.y, .width = CELL_SIZE, .height = CELL_SIZE};
                // DrawRectangleRec(rec, active.tetro.color);
                // DrawRectangleLinesEx(rec, 1, BLACK);
            }
            currentPos.x += CELL_SIZE;
        }
        currentPos.y += CELL_SIZE;
    }
}

void DrawShadowPiece()
{
    Vector2 shadowPosGrid = {
        active.positionInGrid.x,
        active.positionInGrid.y};

    while (!CheckCollisionAt(
        shadowPosGrid.x,
        shadowPosGrid.y + 1,
        active.tetro.cellPositions))
    {
        shadowPosGrid.y++;
    }
    Vector2 shadowPos = {
        PLAYFIELD_OFFSET_LEFT + shadowPosGrid.x * CELL_SIZE,
        PLAYFIELD_OFFSET_UP + shadowPosGrid.y * CELL_SIZE};

    for (int i = 0; i < 4; i++)
    {
        shadowPos.x = PLAYFIELD_OFFSET_LEFT + active.positionInGrid.x * CELL_SIZE;
        for (int j = 0; j < 4; j++)
        {
            if (active.tetro.cellPositions[i][j] == 1)
            {
                Rectangle source = {0, 0, active.tetro.blockTexture.width, active.tetro.blockTexture.height};
                Rectangle dest = {shadowPos.x, shadowPos.y, (float)CELL_SIZE, (float)CELL_SIZE};
                Vector2 origin = {0, 0};
                DrawTexturePro(active.tetro.blockTexture, source, dest, origin, 0, (Color){255, 255, 255, 67});
            }
            shadowPos.x += CELL_SIZE;
        }
        shadowPos.y += CELL_SIZE;
    }
}