// windows: gcc -Wall -Wextra -o tetris tetris.c tetromino.c render.c playfield.c input.c -lraylib -lopengl32 -lgdi32 -lwinmm -lm
// linux:   gcc -Wall -Wextra -o tetris tetris.c tetromino.c render.c playfield.c input.c -lraylib -lm
// TODO
// 1. add lock delay - done
// 2. split the code into separate files - done
// 3. make it look prettier with pre drawn sprites - workinprogress
// 4. sound effects and a bgm - (2/?)
// 5. game states- game over, menu, playing - next
// 6. animations for line clear, etc and also background

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <string.h>
#include <time.h>
#include "tetromino.h"
#include "render.h"
#include "playfield.h"
#include "input.h"
#include "audio.h"

int main(void)
{
    srand(time(NULL));

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetoris");
    InitAudioDevice();

    LoadAudio();
    // load textures
    LoadBlockTextures();
    LoadPlayfieldTexture();
    LoadBackgroundTextures(backgroundTextures, &numBackgroundTextures);
    LoadAnimatedBackgroundTextures();

    // make tetromino variables
    InitTetrominos();
    InitPlayfield();

    StartBGM();

    // active tetro
    SpawnNewPiece();

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        UpdateBGM();

        if (!gameOver)
        {
            UpdateScreenShake();
            UpdateActiveTetromino();
            HandleLineClears();
        }

        // DrawBackground(backgroundTextures, numBackgroundTextures); // static bg
        DrawAnimatedBackground();
        DrawPlayfield();
        DrawShadowPiece();
        DrawActiveTetromino();

        if (gameOver)
        {
            DrawGameOver();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}