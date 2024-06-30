#include "Audio.h"

#include <raylib.h>

Sound soundEffects[6];

Music tetrisSwing;

void InitAudio() {
    InitAudioDevice();

    tetrisSwing = LoadMusicStream("assets/tetris-swing.wav");
    PlayMusicStream(tetrisSwing);

    soundEffects[CLEAR] = LoadSound("assets/clear.wav");
    soundEffects[GAME_OVER] = LoadSound("assets/game-over.wav");
    soundEffects[MOVE] = LoadSound("assets/move.wav");
    soundEffects[HARD_DROP] = LoadSound("assets/harddrop.wav");
    soundEffects[SOFT_DROP] = LoadSound("assets/softdrop.wav");
    soundEffects[ROTATE] = LoadSound("assets/rotate.wav");
}

void PlaySoundEffect(enum SoundType soundType) {
    PlaySound(soundEffects[soundType]);
}

void UpdateMusic() {
    UpdateMusicStream(tetrisSwing);
}