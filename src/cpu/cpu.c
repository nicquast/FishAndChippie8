//
// Created by nic on 15/11/25.
//

#include "cpu.h"

#include <stdlib.h>

Node* createNode();

bool stackIsEmpty(Stack* stack) {
    if (stack == nullptr) return true;
    return stack->top == nullptr;
}

void stackPush(Stack* stack, u_int16_t memory_address) {
    Node* current_top = stack->top;
    Node* new_top = createNode();
    new_top->next = current_top;
    new_top->memory_address = memory_address;
    stack->top = new_top;
}

u_int16_t stackPop(Stack* stack) {
    Node* current_top = stack->top;
    u_int16_t memory_address = current_top->memory_address;
    Node* new_top = current_top->next;
    stack->top = new_top;
    free(current_top);
    return memory_address;
}

u_int16_t stackPeek(Stack* stack) {
    Node* current_top = stack->top;
    return current_top->memory_address;
}

Node* createNode() {
    Node* node = (Node*)malloc(sizeof(Node));
    node->next = nullptr;
    return node;
}
