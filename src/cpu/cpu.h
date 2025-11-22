//
// Created by nic on 15/11/25.
//

#ifndef FISHANDCHIPPIE8_CPU_H
#define FISHANDCHIPPIE8_CPU_H
#include "../memory/memory.h"

#define NUM_REGISTERS 16



typedef struct Node {
    mem_addr_t memory_address;
    struct Node* next;
} Node;

typedef struct Stack {
    Node* top;
} Stack;

void freeStack(Stack* stack);

void stackPush(Stack* stack, mem_addr_t memory_address);
mem_addr_t stackPop(Stack* stack);
mem_addr_t stackPeek(Stack* stack);
bool stackIsEmpty(Stack* stack);

typedef struct {
    mem_addr_t program_counter;
    mem_addr_t index_register;
    byte_t gp_registers[NUM_REGISTERS];
    byte_t delay_timer;
    byte_t sound_timer;
} RegisterStore;

RegisterStore initRegisterStore();

#endif //FISHANDCHIPPIE8_CPU_H