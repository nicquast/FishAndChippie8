//
// Created by nic on 16/11/25.
//

#ifndef FISHANDCHIPPIE8_SYSTEM_H
#define FISHANDCHIPPIE8_SYSTEM_H

#include "../memory/memory.h"
#include "../cpu/cpu.h"
#include "../display/display.h"

typedef struct {
    RegisterStore register_store;
    byte* memory;
} Chip8System;

typedef struct {
    Chip8System system;
    pixel_t* pixel_buffer;
} SaveState;

Chip8System initChip8System();

#endif //FISHANDCHIPPIE8_SYSTEM_H