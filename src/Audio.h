#ifndef AUDIO_H
#define AUDIO_H

#include <raylib.h>

enum SoundType {
	CLEAR,
	GAME_OVER,
	MOVE,
	HARD_DROP,
	SOFT_DROP,
	ROTATE
};

void InitAudio();
void PlaySoundEffect(enum SoundType soundType);
void UpdateMusic();

#endif