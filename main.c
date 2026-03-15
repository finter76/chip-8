#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"


int main(int argc, char** argv){
    if(argc != 2){
        printf("Usage: %s <ROM>\n", argv[0]);    
        exit(1);
    }
    
    chip8 c;
    init(&c);

    FILE *rom = fopen(argv[1], "rb");
    if(!rom){
        printf("Error opening rom\n");
        exit(1);
    }
    load_rom(&c, rom);

    while(1){
        fetch(&c);
        execute(&c);
    
    }

    return 0;
}
