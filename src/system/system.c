//
// Created by nic on 16/11/25.
//

#include "system.h"

#define getNibble1(instruction) (instruction >> 12)
#define getX(instruction) (instruction >> 8 & 0x0f)
#define getY(instruction) (instruction >> 4 & 0x00f)
#define getN(instruction) (instruction & 0x000f)
#define getNN(instruction) (instruction & 0x00ff)
#define getNNN(instruction) (instruction & 0x0fff)

Chip8System initChip8System() {
    Chip8System system;
    system.memory = initMemory();
    system.register_store = initRegisterStore();
    return system;
}

instruction_t fetchInstruction(Chip8System system) {
    mem_addr_t pc = system.register_store.program_counter;
    byte_t byte1 = system.memory[pc];
    byte_t byte2 = system.memory[pc + 1];

    // Concatenate the two bytes making up an instruction
    instruction_t instruction = ((instruction_t)byte1 << 8) | byte2;

    system.register_store.program_counter += 2;

    return instruction;
}