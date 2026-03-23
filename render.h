#ifndef RENDER_H
#define RENDER_H

#include <raylib.h>
#include <stdint.h>
#include "constants.h"

#define TEXTURE_BLOCK_SKYBLUE "textures/blocks/skyblue.png"
#define TEXTURE_BLOCK_BLUE "textures/blocks/blue.png"
#define TEXTURE_BLOCK_ORANGE "textures/blocks/orange.png"
#define TEXTURE_BLOCK_YELLOW "textures/blocks/yellow.png"
#define TEXTURE_BLOCK_GREEN "textures/blocks/green.png"
#define TEXTURE_BLOCK_PURPLE "textures/blocks/purple.png"
#define TEXTURE_BLOCK_RED "textures/blocks/red.png"

#define BACKGROUND_FOLDER "textures/background/Clouds 3"

#define TEXTURE_PLAYFIELD "textures/playfield2.png"

extern Texture2D textureBlockSkyblue;
extern Texture2D textureBlockBlue;
extern Texture2D textureBlockOrange;
extern Texture2D textureBlockYellow;
extern Texture2D textureBlockGreen;
extern Texture2D textureBlockPurple;
extern Texture2D textureBlockRed;

extern Texture2D playfieldTexture;

extern int numBackgroundTextures;
extern Texture2D backgroundTextures[100];

// screen shake
extern float shakeTimer;
extern Vector2 shakeOffset;

void LoadBlockTextures();
void LoadPlayfieldTexture();
void LoadBackgroundTextures(Texture* textures, int* num_textures);
void LoadAnimatedBackgroundTextures();

void DrawBackground(Texture* textures, int num_textures);
void DrawAnimatedBackground();
void DrawActiveTetromino();
void DrawShadowPiece();
void DrawPlayfield();
void DrawGameOver();

void UpdateScreenShake();

#endif