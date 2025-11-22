//
// Created by nic on 16/11/25.
//

#ifndef FISHANDCHIPPIE8_SYSTEM_H
#define FISHANDCHIPPIE8_SYSTEM_H

#include "../memory/memory.h"
#include "../cpu/cpu.h"
#include "../display/display.h"

typedef u_int16_t instruction_t;

typedef struct {
    RegisterStore register_store;
    byte_t* memory;
} Chip8System;

typedef struct {
    Chip8System system;
    pixel_t* pixel_buffer;
} SaveState;

Chip8System initChip8System();

// Fetches the next instruction and increments PC
instruction_t fetchInstruction(Chip8System system);

#endif //FISHANDCHIPPIE8_SYSTEM_H