// windows: gcc -Wall -Wextra -o tetris tetris.c tetromino.c render.c playfield.c input.c gamestate.c -lraylib -lopengl32 -lgdi32 -lwinmm -lm
// linux:   gcc -Wall -Wextra -o tetris tetris.c tetromino.c render.c playfield.c input.c gamestate.c -lraylib -lm
// TODO
// 1. add lock delay - done
// 2. split the code into separate files - done
// 3. make it look prettier with pre drawn sprites - workinprogress
// 4. sound effects and a bgm - (2/?)
// 5  improve input handling, make it more responsive....... DONE
// 6. game states- game over, menu, playing - next
// 7. animations for line clear, etc and also background

// **** VISUAL BUG: remove padding for non existent upper border for the playfield

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
#include "gamestate.h"

int main(void)
{
    srand(time(NULL));

    SetConfigFlags(FLAG_WINDOW_HIGHDPI); // for stupid ass scaling issues
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetoris");
    InitAudioDevice();

    LoadAudio();

    // load textures
    LoadBlockTextures();
    LoadPlayfieldTexture();
    LoadBackgroundTextures(backgroundTextures, &numBackgroundTextures);
    LoadAnimatedBackgroundTextures();
    LoadMenuTextures();

    // make tetromino variables
    InitTetrominos();

    StartBGM();

    // start game
    StartGame();

    SetTargetFPS(FRAME_RATE);

    currentGameState = STATE_MENU;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        UpdateBGM();

        switch (currentGameState) {

        case STATE_MENU:
            DrawAnimatedBackground();

            DrawMenu();
            HandleGameOver();
            // handlemenu()
            break;

        case STATE_PLAYING:
            UpdateScreenShake();
            UpdateActiveTetromino();
            HandleLineClears();

            // DrawBackground(backgroundTextures, numBackgroundTextures); // static bg
            DrawAnimatedBackground();
            DrawPlayfield();
            DrawShadowPiece();
            DrawActiveTetromino();
            break;

        case STATE_OVER:
            // DrawBackground(backgroundTextures, numBackgroundTextures); // static bg
            DrawAnimatedBackground();
            DrawPlayfield();
            DrawShadowPiece();
            DrawActiveTetromino();

            DrawGameOver();
            HandleGameOver();
            break;

        case STATE_PAUSED:
            break;

        default:
            exit(-1);

        }
        // if (currentGameState != STATE_OVER)
        // {
        //     UpdateScreenShake();
        //     UpdateActiveTetromino();
        //     HandleLineClears();
        // }

        // // DrawBackground(backgroundTextures, numBackgroundTextures); // static bg
        // DrawAnimatedBackground();
        // DrawPlayfield();
        // DrawShadowPiece();
        // DrawActiveTetromino();

        // if (currentGameState == STATE_OVER)
        // {
        //     DrawGameOver();
        // }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}