#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include "chip8.h"

#define SCALE 10

typedef struct display{
    SDL_Window* window;
    SDL_Renderer* context;
} display;

void init_display(display*);
void rendering(display*, chip8*);
void cleanup(display*);

#endif
