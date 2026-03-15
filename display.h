#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include "chip8.h"

#define SCALE 15
#define FPS 60
#define MILLISECS_PER_FRAME (1000/FPS)
#define INSTRUCTIONS_PER_FRAME 12 

typedef struct display{
    SDL_Window* window;
    SDL_Renderer* context;
} display;

void init_display(display*);
void rendering(display*, chip8*);
void cleanup(display*);

#endif
