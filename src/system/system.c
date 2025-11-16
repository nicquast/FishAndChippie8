//
// Created by nic on 16/11/25.
//

#include "system.h"

Chip8System initChip8System() {
    Chip8System system;
    system.memory = initMemory();
    system.register_store = initRegisterStore();
    return system;
}