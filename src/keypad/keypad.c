//
// Created by nic on 10/12/25.
//
#include <SDL3/SDL_keyboard.h>
#include "keypad.h"

void updateKeypad(bool* keypad) {
    const bool* keyboard_state = SDL_GetKeyboardState(nullptr);

    for (int i = 0; i < KEYPAD_ARRAY_SIZE; i++) {
        const SDL_Scancode scancode = keymap[i];
        keypad[i] = keyboard_state[scancode];
    }
}