#include <raylib.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include "render.h"
#include "constants.h"
#include "tetromino.h"
#include "playfield.h"

Texture2D textureBlockSkyblue;
Texture2D textureBlockBlue;
Texture2D textureBlockOrange;
Texture2D textureBlockYellow;
Texture2D textureBlockGreen;
Texture2D textureBlockPurple;
Texture2D textureBlockRed;

Texture2D playfieldTexture;

int numBackgroundTextures = 0;
Texture2D backgroundTextures[100];

Texture2D animatedBg1, animatedBg2, animatedBg3, animatedBg4, animatedBg5;
float scrolling2 = 0.0f;
float scrolling3 = 0.0f;
float scrolling4 = 0.0f;
float scrolling5 = 0.0f;

float shakeTimer = 0.0f;
Vector2 shakeOffset = { 0, 0 };


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

void LoadPlayfieldTexture()
{
    playfieldTexture = LoadTexture(TEXTURE_PLAYFIELD);
}

void LoadBackgroundTextures(Texture* textures, int* num_textures)
{
    DIR* dir = opendir(BACKGROUND_FOLDER);
    if (dir == NULL)
        return;

    struct dirent* entry;
    char full_path[PATH_MAX * 2 + 1];
    int i = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", BACKGROUND_FOLDER, entry->d_name);

        textures[i++] = LoadTexture(full_path);
    }

    *num_textures = i;

    closedir(dir);
}

void LoadAnimatedBackgroundTextures()
{
    // just a test, so hardcoded paths
    animatedBg1 = LoadTexture("textures/anim_background/2/Night/1.png");
    animatedBg2 = LoadTexture("textures/anim_background/2/Night/2.png");
    animatedBg3 = LoadTexture("textures/anim_background/2/Night/3.png");
    animatedBg4 = LoadTexture("textures/anim_background/2/Night/4.png");
    animatedBg5 = LoadTexture("textures/anim_background/2/Night/5.png");
}

void DrawBackground(Texture* textures, int num_textures)
{
    for (int i = 0; i < num_textures; i++)
    {
        Rectangle source = { 0, 0, textures[i].width, textures[i].height };
        Rectangle dest = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
        Vector2 origin = { 0, 0 };
        DrawTexturePro(textures[i], source, dest, origin, 0, (Color) { 255, 255, 255, 255 });
    }
}

void DrawAnimatedBackground()
{
    float dt = GetFrameTime();

    scrolling2 -= 0.2f * dt * 60;
    scrolling3 -= 0.4f * dt * 60;
    scrolling4 -= 0.7f * dt * 60;
    scrolling5 -= 1.0f * dt * 60;

    if (scrolling2 <= -animatedBg2.width * BG_SCALE) scrolling2 = 0;
    if (scrolling3 <= -animatedBg3.width * BG_SCALE) scrolling3 = 0;
    if (scrolling4 <= -animatedBg4.width * BG_SCALE) scrolling4 = 0;
    if (scrolling5 <= -animatedBg5.width * BG_SCALE) scrolling5 = 0;

    DrawTextureEx(animatedBg1, (Vector2) { 0, 0 }, 0.0f, BG_SCALE, WHITE);

    DrawTextureEx(animatedBg2, (Vector2) { scrolling2, 0 }, 0.0f, BG_SCALE, WHITE);
    DrawTextureEx(animatedBg2, (Vector2) { animatedBg2.width* BG_SCALE + scrolling2, 0 }, 0.0f, BG_SCALE, WHITE);

    DrawTextureEx(animatedBg3, (Vector2) { scrolling3, 0 }, 0.0f, BG_SCALE, WHITE);
    DrawTextureEx(animatedBg3, (Vector2) { animatedBg3.width* BG_SCALE + scrolling3, 0 }, 0.0f, BG_SCALE, WHITE);

    DrawTextureEx(animatedBg4, (Vector2) { scrolling4, 0 }, 0.0f, BG_SCALE, WHITE);
    DrawTextureEx(animatedBg4, (Vector2) { animatedBg4.width* BG_SCALE + scrolling4, 0 }, 0.0f, BG_SCALE, WHITE);

    DrawTextureEx(animatedBg5, (Vector2) { scrolling5, 0 }, 0.0f, BG_SCALE, WHITE);
    DrawTextureEx(animatedBg5, (Vector2) { animatedBg5.width* BG_SCALE + scrolling5, 0 }, 0.0f, BG_SCALE, WHITE);

}

void DrawActiveTetromino()
{
    Vector2 currentPos = {
        PLAYFIELD_OFFSET_LEFT + shakeOffset.x + active.positionInGrid.x * CELL_SIZE,
        PLAYFIELD_OFFSET_UP + shakeOffset.y + active.positionInGrid.y * CELL_SIZE
    };

    for (int i = 0; i < 4; i++)
    {
        currentPos.x = PLAYFIELD_OFFSET_LEFT + active.positionInGrid.x * CELL_SIZE;
        for (int j = 0; j < 4; j++)
        {
            if (active.tetro.cellPositions[i][j] == 1)
            {
                Rectangle source = { 0, 0, active.tetro.blockTexture.width, active.tetro.blockTexture.height };
                Rectangle dest = { currentPos.x, currentPos.y, (float)CELL_SIZE, (float)CELL_SIZE };
                Vector2 origin = { 0, 0 };
                DrawTexturePro(active.tetro.blockTexture, source, dest, origin, 0, (Color) { 255, 255, 255, 255 });
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
        active.positionInGrid.y };

    while (!CheckCollisionAt(
        shadowPosGrid.x,
        shadowPosGrid.y + 1,
        active.tetro.cellPositions))
    {
        shadowPosGrid.y++;
    }
    Vector2 shadowPos = {
        PLAYFIELD_OFFSET_LEFT + shakeOffset.x + shadowPosGrid.x * CELL_SIZE,
        PLAYFIELD_OFFSET_UP + shakeOffset.y + shadowPosGrid.y * CELL_SIZE
    };

    for (int i = 0; i < 4; i++)
    {
        shadowPos.x = PLAYFIELD_OFFSET_LEFT + active.positionInGrid.x * CELL_SIZE;
        for (int j = 0; j < 4; j++)
        {
            if (active.tetro.cellPositions[i][j] == 1)
            {
                Rectangle source = { 0, 0, active.tetro.blockTexture.width, active.tetro.blockTexture.height };
                Rectangle dest = { shadowPos.x, shadowPos.y, (float)CELL_SIZE, (float)CELL_SIZE };
                Vector2 origin = { 0, 0 };
                DrawTexturePro(active.tetro.blockTexture, source, dest, origin, 0, (Color) { 255, 255, 255, 67 });
            }
            shadowPos.x += CELL_SIZE;
        }
        shadowPos.y += CELL_SIZE;
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
    Rectangle source = { 0, 0, playfieldTexture.width, playfieldTexture.height };
    Rectangle dest = {
        PLAYFIELD_OFFSET_LEFT - PLAYFIELD_BORDER_SCALED_X + shakeOffset.x,
        PLAYFIELD_OFFSET_UP - PLAYFIELD_BORDER_SCALED_Y + shakeOffset.y,
        PLAYFIELD_GRID_PIXEL_WIDTH + PLAYFIELD_BORDER_SCALED_X * 2,
        PLAYFIELD_GRID_PIXEL_HEIGHT + PLAYFIELD_BORDER_SCALED_Y * 2
    };
    Vector2 origin = { 0, 0 };

    DrawTexturePro(playfieldTexture, source, dest, origin, 0, (Color) { 255, 255, 255, 190 });
    // -------

    // draw existing blocks
    for (int i = 0; i < PLAYFIELD_HEIGHT; i++)
    {
        for (int j = 0; j < PLAYFIELD_WIDTH; j++)
        {
            if (playfieldGrid[i][j].occupied)
            {
                Vector2 occupiedPos = {
                    PLAYFIELD_OFFSET_LEFT + j * CELL_SIZE + shakeOffset.x,
                    PLAYFIELD_OFFSET_UP + i * CELL_SIZE + shakeOffset.y
                };

                Rectangle source = { 0, 0, playfieldGrid[i][j].blockTexture.width, playfieldGrid[i][j].blockTexture.height };
                Rectangle dest = { occupiedPos.x, occupiedPos.y, (float)CELL_SIZE, (float)CELL_SIZE };
                Vector2 origin = { 0, 0 };
                DrawTexturePro(playfieldGrid[i][j].blockTexture, source, dest, origin, 0, (Color) { 255, 255, 255, 255 });
            }
        }
    }

    // DrawLineEx(leftLineStartPos, leftLineEndPos, PLAYFIELD_BORDER_THICKNESS, RAYWHITE);
    // DrawLineEx(rightLineStartPos, rightLineEndPos, PLAYFIELD_BORDER_THICKNESS, RAYWHITE);
    // DrawLineEx(downLineStartPos, downLineEndPos, PLAYFIELD_BORDER_THICKNESS, RAYWHITE);
}

void DrawGameOver()
{
    const char* text = "GAME OVER";
    int textWidth = MeasureText(text, FONT_SIZE);
    DrawText(text, WINDOW_WIDTH / 2 - textWidth / 2, WINDOW_HEIGHT / 2, FONT_SIZE, WHITE);
}

void UpdateScreenShake()
{
    if (shakeTimer > 0)
    {
        shakeTimer -= GetFrameTime();
        float intensity = SCREEN_SHAKE_INTENSITY; // pixels
        shakeOffset.x = (float)(rand() % (int)(intensity * 2)) - intensity;
        shakeOffset.y = (float)(rand() % (int)(intensity * 2)) - intensity;
    }
    else
    {
        shakeOffset = (Vector2){ 0, 0 };
    }
}
