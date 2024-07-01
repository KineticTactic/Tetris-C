#ifndef AUDIO_H
#define AUDIO_H

#include <raylib.h>

enum SoundType { CLEAR, GAME_OVER, MOVE, HARD_DROP, SOFT_DROP, ROTATE };

void audio_init();
void audio_play(enum SoundType soundType);
void audio_update();

#endif