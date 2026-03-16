#include "audio.h"
#include <SDL2/SDL.h>

#define SAMPLE_RATE 44100
#define AMPLITUDE   28000
#define FREQ        440

static int phase  = 0;
static int period = SAMPLE_RATE / FREQ;

void audio_callback(void *userdata, Uint8 *stream, int len){
    (void)userdata; // La firma della funzione è bloccata, devo scrivere userdata nella funzione, senza usarlo. Serve per mandare le impostazioni per disegnare l'onda sonora, ma le dichiaro e uso in questo file e non ho bisogno di passarle
    Sint16 *buf = (Sint16*)stream; // Buffer da riempire, campioni a 16 bit
    int samples = len / 2; // Len è in byte, ogni campione ono 2 byte
    
    // Disegno un'onda quadra, con duty cicle = 1/2
    /*
+28000 |‾‾‾‾‾|     |‾‾‾‾‾|     |‾‾‾‾‾
       |     |     |     |     |
-28000 |     |_____|     |_____|
        ←100→
    */
    for(int i = 0; i < samples; i++){
        buf[i] = (phase < period/2) ? AMPLITUDE : -AMPLITUDE;
        phase = (phase + 1) % period;
    }
}

void init_audio(){
    SDL_AudioSpec spec = { 0 };
    spec.freq = SAMPLE_RATE; // Campioni al secondo
    spec.format = AUDIO_S16SYS; // Campioni signed 16 bit
    spec.channels = 1; // Audio mono
    spec.samples = 512; // Dimensione buffer interno
    spec.callback = audio_callback;
    SDL_OpenAudio(&spec, NULL);
    SDL_PauseAudio(1); // Parte in silenzio
}

void update_audio(chip8 *c){
    if(c->sound_timer > 0) SDL_PauseAudio(0);
    else                   SDL_PauseAudio(1);
}

void cleanup_audio(){
    SDL_CloseAudio();
}
