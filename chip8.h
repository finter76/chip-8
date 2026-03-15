#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <stdio.h>

#define DISP_WIDTH 64
#define DISP_HEIGHT 32
#define MEM_SIZE 4096
#define STACK_SIZE 16
#define NUM_REGS 16
#define NUM_KEYS 16
#define FONT_OFFSET 0x050
#define ROM_OFFSET 0x200
#define OPCODE_SIZE 2

typedef struct {
    unsigned char key[NUM_KEYS];
    
    //0x000-0x1FF - Riservato all'interprete
    //0x050-0x0A0 - Fontset built-in 4x5 pixel per carattere (0-F)
    //0x200-0xFFF - ROM + RAM del programma
    unsigned char memory[MEM_SIZE];

    unsigned char V[NUM_REGS]; // Registri generali V0-VF

    unsigned short I; // Registro indirizzo
    unsigned short pc; // Program Counter

    unsigned short opcode; // Opcode corrente, 2 byte

    unsigned char gfx[DISP_WIDTH * DISP_HEIGHT]; // Gestione Display

    unsigned char draw_flag; //Segnala che il display va ridisengato

    unsigned char delay_timer; // Decrementato a 60 Hz, uso generico
    unsigned char sound_timer; // Decrementato a 60 Hz, emette beep quando > 0

    unsigned short stack[STACK_SIZE]; // Stack per indirizzi di ritorno
    unsigned short sp; // Stack pointer
} chip8;

void init(chip8 *c);
void load_rom(chip8 *c, FILE*);
void fetch(chip8 *c);
void execute(chip8 *c);

#endif
