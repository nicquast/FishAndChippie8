//
// Created by nic on 15/11/25.
//

#include "cpu.h"

#include <stdlib.h>
#include <string.h>

Node* createNode();

bool stackIsEmpty(Stack* stack) {
    if (stack == nullptr) return true;
    return stack->top == nullptr;
}

void stackPush(Stack* stack, mem_addr_t memory_address) {
    Node* current_top = stack->top;
    Node* new_top = createNode();
    new_top->next = current_top;
    new_top->memory_address = memory_address;
    stack->top = new_top;
}

mem_addr_t stackPop(Stack* stack) {
    Node* current_top = stack->top;
    mem_addr_t memory_address = current_top->memory_address;
    Node* new_top = current_top->next;
    stack->top = new_top;
    free(current_top);
    return memory_address;
}

mem_addr_t stackPeek(Stack* stack) {
    Node* current_top = stack->top;
    return current_top->memory_address;
}

Node* createNode() {
    Node* node = (Node*)malloc(sizeof(Node));
    node->next = nullptr;
    return node;
}

RegisterStore initRegisterStore() {
    RegisterStore register_store;
    memset(register_store.gp_registers, 0, sizeof(byte_t) * NUM_REGISTERS);
    register_store.program_counter = 0;
    register_store.index_register = 0;
    register_store.sound_timer = 0;
    register_store.delay_timer = 0;
    return register_store;
}
