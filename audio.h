#ifndef AUDIO_H
#define AUDIO_H

#include "chip8.h"

void init_audio();
void update_audio(chip8 *c);
void cleanup_audio();

#endif
