#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include "input.h"
#include "display.h"
#include "time.h"

int main(int argc, char** argv){
    if(argc < 1) {
        printf("Usage: <BINARY> <CHIP-8 ROM>\n");
        exit(1);
    } else if(argc < 2){
        printf("Usage: %s <ROM>\n", argv[0]);    
        exit(1);
    }
    
    chip8 c;
    init(&c);

    display d;
    init_display(&d);

    FILE *rom = fopen(argv[1], "rb");
    if(!rom){
        printf("Error opening rom\n");
        exit(1);
    }
    load_rom(&c, rom);    

    fclose(rom);

    srand(time(NULL));
    
    while(1){
        Uint32 frame_start = SDL_GetTicks();
        
        input_handler(&c);

        for(int i = 0; i < INSTRUCTIONS_PER_FRAME; i++){
            fetch(&c);
            execute(&c);
        }
        
        if(!c.waiting_key){
            if(c.delay_timer > 0) c.delay_timer--;
            if(c.sound_timer > 0) c.sound_timer--;
        }

        if(c.draw_flag){
            rendering(&d, &c);
            c.draw_flag = 0;
        }
        
        Uint32 frame_time = SDL_GetTicks() - frame_start;
        if(frame_time < MILLISECS_PER_FRAME)
            SDL_Delay(MILLISECS_PER_FRAME - frame_time);
    }
    
    cleanup(&d);

    return 0;
}
