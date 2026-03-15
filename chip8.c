#include "chip8.h"
#include <string.h>
#include <stdlib.h>

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

    if(size > MEM_SIZE - ROM_OFFSET){
        printf("ROM too much big: %ld\n", size);
        exit(1);
    }

    // Leggi size elementi grandi da un byte l'uno 
    fread(c->memory+ROM_OFFSET, 1, size, rom);      
}

void fetch(chip8 *c){
    if(!c) return;        
  
    if(c->pc >= MEM_SIZE){
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
                    disp_clear(c);
                break;
            
                case 0xEE:
                    return_to(c);
                break;

                default:
                break;
            }
        break;
        
        case 0x1000:
            jump(c, address);
        break;
        
        case 0x2000:
            call(c, address);
        break;
        
        case 0x3000:
            cmp_equal_const(c, VX, value);
        break;
    
        case 0x4000:
            cmp_inequal_const(c, VX, value);
        break;
        
        case 0x5000:
            cmp_equal_reg(c, VX, VY);
        break;
        
        case 0x6000:
            set_vx_value(c, VX, value);
        break;
        
        case 0x7000:
            sum_vx_value(c, VX, value);
        break;
        
        case 0x8000:
            switch(last_nibble){
                case 0x0:
                    set_vx_vy(c, VX, VY);
                break;

                case 0x1:                    
                    or_vx_vy(c, VX, VY);
                break;        
    
                case 0x2:
                    and_vx_vy(c, VX, VY);
                break;        
                
                case 0x3:
                    xor_vx_vy(c, VX, VY);
                break;        
                
                case 0x4:
                    add_vx_vy(c, VX, VY);
                break;        
                
                case 0x5:
                    sub_vx_vy(c, VX, VY);
                break;        
                
                case 0x6:
                    shiftR_vx_vy(c, VX, VY);
                break;        
                       
                case 0x7:
                    sub_vy_vx(c, VX, VY);
                break;   

                case 0xE:
                    shiftL_vx_vy(c, VX, VY);
                break;     

                default:
                break;
            }
        break;
        
        case 0x9000:
            cmp_inequal_reg(c, VX, VY);
        break;
        
        case 0xA000:
            // Mette I all'indirizzo NNN
            set_i(c, address);
        break;

        case 0xB000:
            // Salta all'indirizzo NNN + V0
            jump_offset(c, address);
        break;
        
        case 0xC000:
            //CXNN: mette il risultato dell'operazione
            // bit a bit tra un valore casuale e NN in Vx
            rand_vx(c, VX, address);
        break;
        
        case 0xD000:
            // Disegna uno sprite alle coordinate (VX, VY) largo 8 pixel e alto N pixel
            // Ogni riga è letta a partire da I
            // I alla fine dell'istruzione non viene toccato
            // VF va a 1 se viene flippato qualche pixel durante questa operazione
            display(c, VX,VY, last_nibble);
        break;
        
        case 0xE000:
            switch(value){
                case 0x9E:
                    cmp_nibble_vx_equal(c, VX);
                break;

                case 0xA1:
                    cmp_nibble_vx_inequal(c, VX);
                break;

                default:
                break;
            }
        break;
        
        case 0xF000:
            switch(value){
                case 0x07:
                    get_delay(c, VX);
                break;

                case 0x0A:
                    get_key(c, VX);
                break;

                case 0x15:
                    delay_timer(c, VX);
                break;

                case 0x18:
                    sound_timer(c, VX);
                break;
                
                case 0x1E:
                    add_i_vx(c, VX);
                break;
        
                case 0x29:
                    sprite_addr(c, VX);    
                break;                

                case 0x33:
                    set_BCD(c, VX);
                break;

                case 0x55:
                    reg_dump(c, VX);
                break;
        
                case 0x65:
                     reg_load(c, VX);
                break;
        
                default:
                break;
            }
        break;
        
        default:
        break;
    }
}

// Gruppo 0
void disp_clear(chip8 *c){
    memset(&(c->gfx), 0, DISP_WIDTH * DISP_HEIGHT);
}
void return_to(chip8 *c){
    c->sp--;
    c->pc = c->stack[c->sp];
}

// Gruppo 1
void jump(chip8 *c, unsigned short address){
    c->pc = address;
}

// Gruppo 2
void call(chip8 *c, unsigned short address){ 
    c->stack[c->sp] = c->pc;
    c->sp++;
    c->pc = address;
}

// Gruppo 3
void cmp_equal_const(chip8 *c, unsigned char vx, unsigned short value){
    if(c->V[vx] == value) c->pc += 2;
}

// Gruppo 4
void cmp_inequal_const(chip8 *c, unsigned char vx, unsigned short value){
    if(c->V[vx] != value) c->pc += 2;
}

// Gruppo 5
void cmp_equal_reg(chip8 *c, unsigned char vx, unsigned char vy){
    if(c->V[vx] == c->V[vy]) c->pc += 2;
}

// Gruppo 6
void set_vx_value(chip8 *c, unsigned char vx, unsigned short value){
    c->V[vx] = value;
}

// Gruppo 7
void sum_vx_value(chip8 *c, unsigned char vx, unsigned short value){
    c->V[vx] += value;
}

// Gruppo 8
void set_vx_vy(chip8 *c, unsigned char vx, unsigned char vy){
    c->V[vx] = c->V[vy];
}
void or_vx_vy(chip8 *c, unsigned char vx, unsigned char vy){
    c->V[vx] = c->V[vx] | c->V[vy];
}
void and_vx_vy(chip8 *c, unsigned char vx, unsigned char vy){
    c->V[vx] = c->V[vx] & c->V[vy];
}
void xor_vx_vy(chip8 *c, unsigned char vx, unsigned char vy){
    c->V[vx] = c->V[vx] ^ c->V[vy];
}
void add_vx_vy(chip8 *c, unsigned char vx, unsigned char vy){
    if(c->V[vx] + c->V[vy] > 255) c->V[0xF] = 1;
    c->V[vx] += c->V[vy];
}
void sub_vx_vy(chip8 *c, unsigned char vx, unsigned char vy){
    if(c->V[vx] >= c->V[vy]) c->V[0xF] = 1;
    c->V[vx] -= c->V[vy];
}

void shiftR_vx_vy(chip8 *c, unsigned char vx){ 
    c->V[0xF] = c->V[vx] & 0x01;
    c->V[vx] >>= 1;
    
}
void sub_vy_vx(chip8 *c, unsigned char vx, unsigned char vy){
    if(c->V[vx] < c->V[vy]) c->V[0xF] = 1;
    c->V[vx] = c->V[vy] - c->V[vx];
}
void shiftL_vx_vy(chip8 *c, unsigned char vx){
    c->V[0xF] = c->V[vx] & 0x80 ? 1 : 0;
    c->V[vx] <<= 1;
    
}

// Gruppo 9
void cmp_inequal_reg(chip8 *c, unsigned char vx, unsigned char vy){
    if(c->V[vx] != c-> V[vy]) c->pc += 2;
}

// Gruppo A
void set_i(chip8 *c, unsigned short address){
    if(address >= MEM_SIZE) return;
    c->I = address;
}

// Gruppo B
void jump_offset(chip8 *c, unsigned short address){
    if((c->V[0] + address) > MEM_SIZE) return; 
    c->pc = c->V[0] + address;
}

// Gruppo C
void rand_vx(chip8 *c, unsigned char vx, unsigned short value){
    c->V[vx] = (rand() & 0xFF) & value;
}

// Gruppo D
void display(chip8 *c, unsigned char vx, unsigned char vy, unsigned char n);

// Gruppo E
void cmp_nibble_vx_equal(chip8 *c, unsigned char vx){
    if(c->key[c->V[vx]]) c->pc += 2;
}
void cmp_nibble_vx_inequal(chip8 *c, unsigned char vx){
    if(!c->key[c->V[vx]]) c->pc += 2;
}

// Gruppo F
void get_delay(chip8 *c, unsigned char vx){
    c->V[vx] = c->delay_timer;
}
void get_key(chip8 *c, unsigned char vx){
    for(int i = 0; i < NUM_KEYS; i++){
        if(c->key[i]){
            c->V[vx] = i;
            return;
        } 
    } 
    c->pc -= 2;
}
void delay_timer(chip8 *c, unsigned char vx){
    c->delay_timer = c->V[vx];
}
void sound_timer(chip8 *c, unsigned char vx){
    c->sound_timer = c->V[vx];
}
void add_i_vx(chip8 *c, unsigned char vx){
    c->I += c->V[vx];
}
// Guardo la cifra in c->V[vx] e metto il suo offset rispetto
// FONT_OFFSET dentro c->I
void sprite_addr(chip8 *c, unsigned char vx){
    c->I = (c->V[vx] * FONT_SIZE) + FONT_OFFSET;
}
// Salva in memoria a partire da I, in codifica BCD, il valore di V[vx] (0-255)
void set_BCD(chip8 *c, unsigned char vx){
    c->memory[c->I] = c->V[vx] / 100;
    
    c->memory[c->I+1] = (c->V[vx] / 10) % 10;
    c->memory[c->I+2] = c->V[vx] % 10;
}
void reg_dump(chip8 *c, unsigned char vx){
    for(unsigned char i = 0; i <= vx; i++){
        c->memory[c->I + i] = c->V[i];
    }
}
void reg_load(chip8 *c, unsigned char vx){
    for(unsigned char i = 0; i <= vx; i++){
        c->V[i] = c->memory[c->I + i];
    }
}
