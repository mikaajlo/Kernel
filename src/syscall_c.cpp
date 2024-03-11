//
// Created by os on 22/08/23.
//

#include "../h/syscall_c.hpp"

void *mem_alloc(size_t size) {
    void* returnValue;
    __asm__ volatile("mv a1, %[size]" : : [size] "r" (size));
    __asm__ volatile("li a0, 0x01");
    __asm__ volatile ("ecall");
    __asm__ volatile("mv %[returnValue], a0" : [returnValue] "=r" (returnValue));
    return returnValue;
}

int mem_free(void *addr) {
    int returnValue;
    __asm__ volatile ("mv a1, %[addr]" : : [addr] "r" (addr));
    __asm__ volatile("li a0, 0x02");
    __asm__ volatile ("ecall");
    __asm__ volatile("mv %[returnValue], a0" : [returnValue] "=r" (returnValue));
    return returnValue;
}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {
    int returnValue;
    char* stack = (char*) mem_alloc(DEFAULT_STACK_SIZE);
    if (!stack) return -1;
    else {
        stack += DEFAULT_STACK_SIZE;
        stack--;
    }

    __asm__ volatile("mv a6, %[stack]" : : [stack] "r" (stack));
    __asm__ volatile("mv a3, %[arg]" : : [arg] "r" (arg));
    __asm__ volatile("mv a2, %[start_routine]" : : [start_routine] "r" (start_routine));
    __asm__ volatile("mv a1, %[handle]" : : [handle] "r" (handle));
    __asm__ volatile("li a0, 0x11");
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[returnValue], a0" : [returnValue] "=r" (returnValue));
    return returnValue;
}

int thread_exit() {
    int returnValue;
    __asm__ volatile("li a0, 0x12");
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[returnValue], a0" : [returnValue] "=r" (returnValue));
    return returnValue;
}

void thread_dispatch() {
    __asm__ volatile("li a0, 0x13");
    __asm__ volatile("ecall");
}

void thread_join(thread_t handle) {
    __asm__ volatile("mv a1, %[handle]" : : [handle] "r" (handle));
    __asm__ volatile("li a0, 0x14");
    __asm__ volatile("ecall");
}

int sem_open(sem_t *handle, unsigned int init) {
    int returnValue;
    __asm__ volatile("mv a2, %[init]" : : [init] "r" (init));
    __asm__ volatile("mv a1, %[handle]" : : [handle] "r" (handle));
    __asm__ volatile("li a0, 0x21");
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[returnValue], a0" : [returnValue] "=r" (returnValue));
    return returnValue;
}

int sem_close(sem_t handle) {
    int returnValue;
    __asm__ volatile("mv a1, %[handle]" : : [handle] "r" (handle));
    __asm__ volatile("li a0, 0x22");
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[returnValue], a0" : [returnValue] "=r" (returnValue));
    return returnValue;
}

int sem_wait(sem_t id) {
    int returnValue;
    __asm__ volatile("mv a1, %[id]" : : [id] "r" (id));
    __asm__ volatile("li a0, 0x23");
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[returnValue], a0" : [returnValue] "=r" (returnValue));
    return returnValue;
}

int sem_signal(sem_t id) {
    int returnValue;
    __asm__ volatile("mv a1, %[id]" : : [id] "r" (id));
    __asm__ volatile("li a0, 0x24");
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[returnValue], a0" : [returnValue] "=r" (returnValue));
    return returnValue;
}

char getc() {
    char returnValue;
    __asm__ volatile("li a0, 0x41");
    __asm__ volatile("ecall");
    __asm__ volatile("mv %[returnValue], a0" : [returnValue] "=r" (returnValue));
    return returnValue;
}

void putc(char c) {
    __asm__ volatile("mv a1, %[c]" : : [c] "r" (c));
    __asm__ volatile("li a0, 0x42");
    __asm__ volatile("ecall");
}