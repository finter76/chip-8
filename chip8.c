#include "chip8.h"
#include <string.h>

// Ogni carattere sta su 5 byte, un byte per riga
// Codifico tutte le righe in esadecimale per la stampa dellecifre da 0 a F
static unsigned char fontset[80] = {
    // 0
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    // 1
    0x20, 0x60, 0x20, 0x20, 0x70,
    // 2
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    // 3
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    // 4
    0x90, 0x90, 0xF0, 0x10, 0x10,
    // 5
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    // 6
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    // 7
    0xF0, 0x10, 0x20, 0x40, 0x40,
    // 8 
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    // 9
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    // A
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    // B
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    // C
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    // D
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    // E 
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    // F
    0xF0, 0x80, 0xF0, 0x80, 0x80
};

// Inizializzo tutti i registri
// Inizializzo IP e carico i font 0-F ai rispettivi indirizzi
void init(chip8 *c){
    memset(c, 0, sizeof(chip8));
    c->pc = 0x200;
    // A partire da 0x050 copio i fontset
    memcpy(c->memory + FONT_OFFSET, fontset, 80);    
}

// Carichiamo la ROM a partire da 0x200
void load_rom(chip8 *c, FILE *rom){
    if(!c) return;

    // Controllo la dimensione della ROM 
    fseek(rom, 0, SEEK_END);
    long size = ftell(rom);
    fseek(rom, 0, SEEK_SET);

    if(size > MEMORY_SIZE - ROM_OFFSET){
        printf("ROM too much big: %d\n", size);
        exit(1);
    }

    // Leggi size elementi grandi da un byte l'uno 
    fread(c->memory+ROM_OFFSET, 1, size, rom);      
}

void fetch(chip8 *c){
    if(!c) return;        
  
    if(pc >= MEM_SIZE){
        printf("Out of Memory. \n");
        exit(1);
    }
    unsigned char opcode_high = c->memory[c->pc];
    unsigned char opcode_low = c->memory[c->pc+1];
    c->pc += 2;
    c->opcode = (opcode_high << 8) + opcode_low;
}

void execute(chip8 *c){
    if(!c) return;

    // OPCODE: 0xXYZW
    // X: Tipo istruzione (gruppo)
    // X: indice registro Vx
    // Y: indice registro Vy
    // ZW: byte immediato, indicato con kk
    // W: distingue istruzione dentro lo stesso gruppo, indicato con n
    // YZW: indirizzo a 12 bit, indicato con nnn

    unsigned short group = c->opcode & 0xF000;
    unsigned short address = c->opcode & 0x0FFF;
    unsigned short value = c->opcode & 0x00FF;
    unsigned char VX = (c->opcode >> 8) & 0xF;
    unsigned char VY = (c->opcode >> 4) & 0xF;
    unsigned char last_nibble = c->opcode & 0x000F;

    switch(group){
        case 0x0000:
            switch(value){
                case 0xE0:
                    disp_clear();
                break;
            
                case 0xEE:
                    return_to();
                break;

                default:
                break;
            }
        break;
        
        case 0x1000:
            jump(address);
        break;
        
        case 0x2000:
            call(address);
        break;
        
        case 0x3000:
            cmp_equal_const(VX, value)
        break;
    
        case 0x4000:
            cmp_inequal_const(VX, value);
        break;
        
        case 0x5000:
            cmp_equal_reg(VX, VY);
        break;
        
        case 0x6000:
            set_vx_value(VX, value);
        break;
        
        case 0x7000:
            sum_vx_value(VX, value);
        break;
        
        case 0x8000:
            switch(last_nibble){
                case 0x0:
                    set_vx_vy(VX, VY);
                break;

                case 0x1:                    
                    or_vx_vy(VX, VY);
                break;        
    
                case 0x2:
                    and_vx_vy(VX, VY);
                break;        
                
                case 0x3:
                    xor_vx_vy(VX, VY);
                break;        
                
                case 0x4:
                    add_vx_vy(VX, VY);
                break;        
                
                case 0x5:
                    sub_vx_vy(VX, VY);
                break;        
                
                case 0x6:
                    shiftR_vx_vy(VX, VY);
                break;        
                       
                case 0x7:
                    sub_vy_vx(VX, VY);
                break;   

                case 0xE:
                    shiftL_vx_vy(VX, VY);
                break;     

                default
                break;
            }
        break;
        
        case 0x9000:
            cmp_inequal_reg(VX, VY);
        break;
        
        case 0xA000:
            // Mette I all'indirizzo NNN
            set_i(address);
        break;

        case 0xB000:
            // Salta all'indirizzo NNN + V0
            jump_offset(address);
        break;
        
        case 0xC000:
            //CXNN: mette il risultato dell'operazione
            // bit a bit tra un valore casuale e NN in Vx
            rand(VX, address);
        break;
        
        case 0xD000:
            // Disegna uno sprite alle coordinate (VX, VY) largo 8 pixel e alto N pixel
            // Ogni riga è letta a partire da I
            // I alla fine dell'istruzione non viene toccato
            // VF va a 1 se viene flippato qualche pixel durante questa operazione
            display(VX,VY, last_nibble)
        break;
        
        case 0xE000:
            switch(value){
                case 0x9E:
                break;

                case 0xA1:
                break;
            }
        break;
        
        case 0xF000:
            switch(value){
                case 0x07:
                    get_delay(VX);
                break;

                case 0x0A:
                    get_key(VX);
                break;

                case 0x15:
                    delay_timer(VX);
                break;

                case 0x18:
                    sound_timer(VX);
                break;
                
                case 0x1E:
                    add_i_vx(VX);
                break;
        
                case 0x29:
                    sprite_addr(VX);    
                break;                

                case 0x33:
                    set_BCD(VX);
                break;

                case 0x55:
                    reg_dump(VX);
                break;
        
                case 0x65:
                     reg_load(VX);
                break;
            }
        break;
        
        default:
        break;
    }
}
