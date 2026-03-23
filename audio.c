#include "audio.h"

Sound hardDropSFX;
Music BGM1;

void LoadAudio()
{
    hardDropSFX = LoadSound(HARDDROP_SFX);
    BGM1 = LoadMusicStream(BGM_EASYGOING);
}

void StartBGM()
{
    PlayMusicStream(BGM1);
}

void UpdateBGM()
{
    UpdateMusicStream(BGM1);
}