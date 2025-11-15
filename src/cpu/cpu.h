//
// Created by nic on 15/11/25.
//

#ifndef FISHANDCHIPPIE8_CPU_H
#define FISHANDCHIPPIE8_CPU_H
#include <sys/types.h>

typedef struct Node {
    u_int16_t memory_address; // ToDo: 16bit int for 16 bit memory addresss - change actual type to a custom defined one later (after memory implementation)
    struct Node* next;
} Node;

typedef struct Stack {
    Node* top;
} Stack;

void freeStack(Stack* stack);

void stackPush(Stack* stack, u_int16_t memory_address);
u_int16_t stackPop(Stack* stack);
u_int16_t stackPeek(Stack* stack);
bool stackIsEmpty(Stack* stack);

#endif //FISHANDCHIPPIE8_CPU_H