#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "chip8.h"
#include "display.h"

void init_display(display* d){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){ 
        printf("Error initialiting SDL2\n");
        exit(1);
    }

    d->window = SDL_CreateWindow(
        "CHIP8", // Titolo
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // Posizione
        DISP_WIDTH * SCALE, DISP_HEIGHT * SCALE, // Dimensione
        0 // Flag
    );

    if(!d->window) exit(1);
 
    d->context = SDL_CreateRenderer(
        d->window, // Finestra
        -1, // Driver
        0 // Flag
    );

    if(!d->context) exit(1);
}

void rendering(display* d, chip8* c){
    if(!d || !c) return;

    SDL_SetRenderDrawColor(d->context, 0, 0, 0, 255); // colore nero
    SDL_RenderClear(d->context); // pulisce

    for(int i = 0; i < DISP_WIDTH * DISP_HEIGHT; i++){
        if(c->gfx[i]){
            int x = (i % DISP_WIDTH) * SCALE;
            int y = (i / DISP_WIDTH) * SCALE;
            SDL_Rect rect = { x, y, SCALE, SCALE };
            SDL_SetRenderDrawColor(d->context, 255, 255, 255, 255); // bianco
            SDL_RenderFillRect(d->context, &rect); 
        }
    }
    SDL_RenderPresent(d->context);
}

void cleanup(display* d){
    if(!d) return;
    
    SDL_DestroyRenderer(d->context);
    SDL_DestroyWindow(d->window);

    SDL_Quit();
}
