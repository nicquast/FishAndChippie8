//
// Created by nic on 16/11/25.
//

#ifndef FISHANDCHIPPIE8_SYSTEM_H
#define FISHANDCHIPPIE8_SYSTEM_H

#include "../memory/memory.h"
#include "../cpu/cpu.h"

typedef struct {
    RegisterStore register_store;
    byte* memory;
} Chip8System;

Chip8System initChip8System();

#endif //FISHANDCHIPPIE8_SYSTEM_H