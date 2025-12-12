//
// Created by nic on 15/11/25.
//

#ifndef FISHANDCHIPPIE8_MEMORY_H
#define FISHANDCHIPPIE8_MEMORY_H
#include <stdio.h>
#include <sys/types.h>

#define MEMORY_LENGTH 4096
#define ROM_START_ADDRESS 0x200
#define FONT_START_ADDRESS 0x0
#define FONT_CHARACTER_OFFSET 5

typedef u_int16_t mem_addr_t;
typedef u_int8_t byte_t;

byte_t* initMemory();
void loadRom(FILE* rom_fp, byte_t* memory);

#endif //FISHANDCHIPPIE8_MEMORY_H