#ifndef AUDIO_H
#define AUDIO_H
#include <raylib.h>

#define HARDDROP_SFX "audio/hardDrop.wav"
#define BGM_EASYGOING "audio/easygoing.wav"
// #define BGM

extern Sound hardDropSFX;
extern Music BGM1;

void LoadAudio();
void StartBGM();
void UpdateBGM();

#endif