//
// Created by os on 08/09/23.
//

#include "../h/Sem.h"

void *Sem::operator new(size_t size) {
    return MemoryAllocator::get().mem_alloc(size);
}

void Sem::operator delete(void *addr) {
    MemoryAllocator::get().mem_free(addr);
}

Sem::Sem(int v) {
    value = v;
}

Sem::~Sem() {
    Blocked* temp = head;
    while (temp) {
        Blocked* old = temp;
        temp = temp->next;
        old->tcb->finished = false;
        old->tcb->closedSem = true;
        Scheduler::put(old->tcb);
        delete old;
    }
}

Sem *Sem::create(int v) {
    return new Sem(v);
}

void Sem::wait() {
    value--;
    if (value < 0) {
        Blocked* newBlocked = new Blocked();
        newBlocked->tcb = TCB::running;
        if (head) {
            tail->next = newBlocked;
            tail = newBlocked;
        }
        else {
            head = newBlocked;
            tail = newBlocked;
        }
        TCB::running->finished = true;
        TCB::dispatch();
    }
}

void Sem::signal() {
    value++;
    if (value <= 0) {
        if (!head) return;
        Blocked* old = head;
        TCB* tcb = head->tcb;
        head = head->next;
        delete old;
        tcb->finished = false;
        Scheduler::put(tcb);
    }
}

void *Sem::Blocked::operator new(size_t size) {
    return MemoryAllocator::get().mem_alloc(size);
}

void Sem::Blocked::operator delete(void *addr) {
    MemoryAllocator::get().mem_free(addr);
}
