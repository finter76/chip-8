#include "input.h"
#include <SDL2/SDL.h>
#include <stdlib.h>

void input_handler(chip8 *c){
    SDL_Event e;

    while(SDL_PollEvent(&e)){
        switch(e.type){
            case SDL_QUIT:
                exit(1);
            break;
    
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym){
                    case SDLK_1: c->key[0x1] = 1; break;
                    case SDLK_2: c->key[0x2] = 1; break;
                    case SDLK_3: c->key[0x3] = 1; break;
                    case SDLK_4: c->key[0xC] = 1; break;
                    case SDLK_q: c->key[0x4] = 1; break;
                    case SDLK_w: c->key[0x5] = 1; break;
                    case SDLK_e: c->key[0x6] = 1; break;
                    case SDLK_r: c->key[0xD] = 1; break;
                    case SDLK_a: c->key[0x7] = 1; break;
                    case SDLK_s: c->key[0x8] = 1; break;
                    case SDLK_d: c->key[0x9] = 1; break;
                    case SDLK_f: c->key[0xE] = 1; break;
                    case SDLK_z: c->key[0xA] = 1; break;
                    case SDLK_x: c->key[0x0] = 1; break;
                    case SDLK_c: c->key[0xB] = 1; break;
                    case SDLK_v: c->key[0xF] = 1; break;
                }
            break;
            
            case SDL_KEYUP:
                switch(e.key.keysym.sym){
                    case SDLK_1: c->key[0x1] = 0; if(c->waiting_key){ c->V[c->waiting_key_reg] = 0x1; c->waiting_key = 0; } break;
                    case SDLK_2: c->key[0x2] = 0; if(c->waiting_key){ c->V[c->waiting_key_reg] = 0x2; c->waiting_key = 0; } break;
                    case SDLK_3: c->key[0x3] = 0; if(c->waiting_key){ c->V[c->waiting_key_reg] = 0x3; c->waiting_key = 0; } break;
                    case SDLK_4: c->key[0xC] = 0; if(c->waiting_key){ c->V[c->waiting_key_reg] = 0xC; c->waiting_key = 0; } break;
                    case SDLK_q: c->key[0x4] = 0; if(c->waiting_key){ c->V[c->waiting_key_reg] = 0x4; c->waiting_key = 0; } break;
                    case SDLK_w: c->key[0x5] = 0; if(c->waiting_key){ c->V[c->waiting_key_reg] = 0x5; c->waiting_key = 0; } break;
                    case SDLK_e: c->key[0x6] = 0; if(c->waiting_key){ c->V[c->waiting_key_reg] = 0x6; c->waiting_key = 0; } break;
                    case SDLK_r: c->key[0xD] = 0; if(c->waiting_key){ c->V[c->waiting_key_reg] = 0xD; c->waiting_key = 0; } break;
                    case SDLK_a: c->key[0x7] = 0; if(c->waiting_key){ c->V[c->waiting_key_reg] = 0x7; c->waiting_key = 0; } break;
                    case SDLK_s: c->key[0x8] = 0; if(c->waiting_key){ c->V[c->waiting_key_reg] = 0x8; c->waiting_key = 0; } break;
                    case SDLK_d: c->key[0x9] = 0; if(c->waiting_key){ c->V[c->waiting_key_reg] = 0x9; c->waiting_key = 0; } break;
                    case SDLK_f: c->key[0xE] = 0; if(c->waiting_key){ c->V[c->waiting_key_reg] = 0xE; c->waiting_key = 0; } break;
                    case SDLK_z: c->key[0xA] = 0; if(c->waiting_key){ c->V[c->waiting_key_reg] = 0xA; c->waiting_key = 0; } break;
                    case SDLK_x: c->key[0x0] = 0; if(c->waiting_key){ c->V[c->waiting_key_reg] = 0x0; c->waiting_key = 0; } break;
                    case SDLK_c: c->key[0xB] = 0; if(c->waiting_key){ c->V[c->waiting_key_reg] = 0xB; c->waiting_key = 0; } break;
                    case SDLK_v: c->key[0xF] = 0; if(c->waiting_key){ c->V[c->waiting_key_reg] = 0xF; c->waiting_key = 0; } break;
                }
            break;
        }
    }
}
