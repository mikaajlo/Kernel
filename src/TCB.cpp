//
// Created by os on 23/08/23.
//

#include "../h/TCB.h"

TCB* TCB::running = nullptr;

void TCB::yield(TCB::Context* oldContext, TCB::Context* newContext) {
    __asm__ volatile("sd ra, (a0)");
    __asm__ volatile("sd sp, 8(a0)");
    __asm__ volatile("ld ra, (a1)");
    __asm__ volatile("ld sp, 8(a1)");
}

TCB::TCB(TCB::Body b, TCB::Argument arg, uint64 *s) {
    body = b;
    argument = arg;
    stack = s;
    context.ra = (uint64)wrapper;
    context.sp = (uint64)s;
    sem = Sem::create(0);
}

TCB* TCB::create(TCB::Body b, TCB::Argument arg, uint64 *s) {
    return new TCB(b, arg, s);
}

void *TCB::operator new(size_t size) {
    return MemoryAllocator::get().mem_alloc(size);
}

void TCB::operator delete(void *addr) {
    running->stack -= DEFAULT_STACK_SIZE;
    running->stack++;
    MemoryAllocator::get().mem_free(running->stack);
    MemoryAllocator::get().mem_free(addr);
}

void TCB::dispatch() {
    TCB* old = running;
    if (old->idle) {
        delete old;
    }
    else {
        if (!old->finished) Scheduler::put(old);
    }
    running = Scheduler::get();

    if (!running) {
        uint64* stack = (uint64*) MemoryAllocator::get().mem_alloc(DEFAULT_STACK_SIZE);
        stack += DEFAULT_STACK_SIZE;
        stack--;
        running = new TCB(idleBody, nullptr, stack);
        running->idle = true;
    }

    yield(&old->context, &running->context);
}

void TCB::wrapper() {
    Riscv::popSppSpie();
    running->body(running->argument);
    thread_exit();
}

void TCB::initialize() {
    running = new TCB(nullptr, nullptr, nullptr);
}

int TCB::exit() {
    if (!running) return -1;
    running->finished = true;
    running->sem->signal();
    dispatch();
    return 0;
}

void TCB::join(TCB* tcb) {
    if (tcb && !tcb->finished) tcb->sem->wait();
}

void TCB::idleBody(void*) {
    while(true);
}
