//
// Created by nic on 16/11/25.
//

#ifndef FISHANDCHIPPIE8_SYSTEM_H
#define FISHANDCHIPPIE8_SYSTEM_H

#include "../memory/memory.h"
#include "../cpu/cpu.h"
#include "../display/display.h"

#define getNibble1(instruction) (instruction >> 12)
#define getX(instruction) (instruction >> 8 & 0x0f)
#define getY(instruction) (instruction >> 4 & 0x00f)
#define getN(instruction) (instruction & 0x000f)
#define getNN(instruction) (instruction & 0x00ff)
#define getNNN(instruction) (instruction & 0x0fff)

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
instruction_t fetchInstruction(Chip8System *system);

#endif //FISHANDCHIPPIE8_SYSTEM_H