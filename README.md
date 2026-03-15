AUDIO DA FINIRE
ROM CONSIGLIATE: https://github.com/JamesGriffin/CHIP-8-Emulator

# 🕹️ CHIP-8 Emulator

Un emulatore CHIP-8 scritto in C con SDL2, capace di eseguire ROM originali come Space Invaders, Pong e Tetris.

---

## 📋 Requisiti

- GCC
- SDL2

**Ubuntu/Debian:**
```bash
sudo apt install gcc libsdl2-dev
```

**macOS (Homebrew):**
```bash
brew install sdl2
```

---

## 🔨 Compilazione

```bash
make
```

---

## 🚀 Utilizzo

```bash
./chip8 <ROM>
```

**Esempio:**
```bash
./chip8 INVADERS
```

---

## ⌨️ Tastiera

La tastiera CHIP-8 originale era un keypad 4x4. Il mapping è il seguente:

```
Tastiera CHIP-8        →    Tastiera PC
┌───┬───┬───┬───┐          ┌───┬───┬───┬───┐
│ 1 │ 2 │ 3 │ C │          │ 1 │ 2 │ 3 │ 4 │
├───┼───┼───┼───┤          ├───┼───┼───┼───┤
│ 4 │ 5 │ 6 │ D │    →     │ Q │ W │ E │ R │
├───┼───┼───┼───┤          ├───┼───┼───┼───┤
│ 7 │ 8 │ 9 │ E │          │ A │ S │ D │ F │
├───┼───┼───┼───┤          ├───┼───┼───┼───┤
│ A │ 0 │ B │ F │          │ Z │ X │ C │ V │
└───┴───┴───┴───┘          └───┴───┴───┴───┘
```

---

## 🏗️ Struttura del progetto

```
chip-8/
├── main.c        # Entry point e game loop
├── chip8.c/h     # Core dell'emulatore (fetch, decode, execute)
├── display.c/h   # Rendering SDL2 e audio
├── input.c/h     # Gestione input da tastiera
└── Makefile
```

---

## ⚙️ Specifiche tecniche

| Parametro | Valore |
|---|---|
| Risoluzione display | 64 × 32 pixel |
| Scala | 15× |
| FPS | 60 |
| Istruzioni per frame | 12 |
| Memoria | 4096 byte |
| Registri generali | 16 (V0–VF) |
| Stack | 16 livelli |
| Tasti | 16 |

---

## 📐 Architettura

Il loop principale esegue ad ogni frame:

1. **Input** — legge gli eventi SDL2 e aggiorna lo stato dei tasti
2. **Execute** — esegue 12 istruzioni (fetch → decode → execute)
3. **Timers** — decrementa `delay_timer` e `sound_timer` a 60 Hz
4. **Render** — ridisegna lo schermo se `draw_flag` è attivo
5. **Audio** — emette un beep finché `sound_timer > 0`

---

## 🎮 ROM compatibili

L'emulatore è compatibile con le ROM CHIP-8 standard. Alcune ROM classiche da provare:

- **INVADERS** — Space Invaders
- **PONG** — Pong
- **TETRIS** — Tetris
- **BRIX** — Breakout clone

Le ROM CHIP-8 sono liberamente reperibili online.
