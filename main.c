#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include "input.h"
#include "display.h"

int main(int argc, char** argv){
    if(argc != 2){
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

    while(1){
        input_handler(&c);

        fetch(&c);
        execute(&c);
        
        if(c.draw_flag){
            rendering(&d, &c);
            c.draw_flag = 0;
        }
    }
    
    cleanup(&d);

    return 0;
}
