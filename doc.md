# minesweeper

### tech's
- C
- Gtk v4

## description
A simple minesweeper game, made in c with Gtk4.

## How Run this program 
You can compile to yourself. I'm using particulary use cmake;

### instructions
- Right Click:  Reveals the slot.
- Left Click:   Put a bomb flag on the slot.
- You lose with your reveal a bomb.
- You win when all the bombs are flagged.

### logic
the `src` has only three files: 
- (1) main: starts the game, and pass a matrix of bombs from game to gui
- (2) game: where the game logic is 
- (3) gui:  where the render mechanism is
i'm considering create a new file to manager interactions between the gui n the game

so i have, peudo-random generated bombs, using rand functions from `stdlib.h`. The bombs are initialized only after user first input, because i need to consider that to create  a possible game. I need to improve the bombs layout on the matrix, sometimes the game has no right decidions.

The information about the bombs are storeed on a matrix, I use a struct the information about each field slot

#### Intersting Mechanisms 
- struct `field_slot`: this struct is declared on game.h. It store everything related to a slot in the game. It has atributes like, bool `is_bomb`, to indentify if there is a bombs on the slot. It's very important, the game matrix store this struct type
- function `reveal_around`: this function is on the `game.c`, and it's a recursive function. It aims to revelead and area, when it's no bombs around. It's very simple, returns if is a bomb or is already revelad, and if it's a zero recusive until find the closest cases.



