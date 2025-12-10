//
// Created by nic on 10/12/25.
//

#ifndef FISHANDCHIPPIE8_KEYPAD_H
#define FISHANDCHIPPIE8_KEYPAD_H

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_scancode.h>

//Keymap
// Maps Chip8 keypad to left side of the keyboard.
// Maps the HEX digit that would be present on the chip8 keypad to the relevant SDL_SCANCODE for where
// It is represented on a keyboard
constexpr SDL_Scancode keymap[] = {
    SDL_SCANCODE_X,
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_W,
    SDL_SCANCODE_E,
    SDL_SCANCODE_A,
    SDL_SCANCODE_S,
    SDL_SCANCODE_D,
    SDL_SCANCODE_Z,
    SDL_SCANCODE_C,
    SDL_SCANCODE_4,
    SDL_SCANCODE_R,
    SDL_SCANCODE_F,
    SDL_SCANCODE_V
};


#endif //FISHANDCHIPPIE8_KEYPAD_H