//
// Created by nic on 16/11/25.
//

#include "system.h"

#include <stdlib.h>


Chip8System initChip8System() {
    Chip8System system = {
        .memory = initMemory(),
        .register_store = initRegisterStore(),
        .stack = {nullptr},
        .keypad = {}
    };
    system.register_store.program_counter = ROM_START_ADDRESS;
    memset(system.keypad, false, KEYPAD_ARRAY_SIZE * sizeof(bool));
    return system;
}

void freeChip8SystemMemory(Chip8System system) {
    free(system.memory);
    freeStack(&system.stack);
    free(system.keypad);
}

instruction_t fetchInstruction(Chip8System *system) {
    mem_addr_t pc = system->register_store.program_counter;
    byte_t byte1 = system->memory[pc];
    byte_t byte2 = system->memory[pc + 1];

    // Concatenate the two bytes making up an instruction
    instruction_t instruction = ((instruction_t)byte1 << 8) | byte2;

    system->register_store.program_counter += 2;

    return instruction;
}