//
// Created by nic on 15/11/25.
//

#ifndef FISHANDCHIPPIE8_CPU_H
#define FISHANDCHIPPIE8_CPU_H
#include <sys/types.h>

typedef u_int16_t mem_addr_t; // ToDo: 16bit int for 16 bit memory addresss - change actual type to a custom defined one later (after memory implementation)


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


#endif //FISHANDCHIPPIE8_CPU_Hasd