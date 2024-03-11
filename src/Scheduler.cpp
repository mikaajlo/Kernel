//
// Created by os on 23/08/23.
//

#include "../h/Scheduler.h"

Scheduler::Node* Scheduler::head = nullptr;
Scheduler::Node* Scheduler::tail = nullptr;

void Scheduler::put(TCB *thread) {
    Node* newNode = new Node();
    newNode->thread = thread;
    if (head) {
        tail->next = newNode;
        tail = newNode;
    }
    else {
        head = newNode;
        tail = newNode;
    }
}

TCB *Scheduler::get() {
    if (head) {
        Node *temp = head;
        head = head->next;
        TCB* tcb = temp->thread;
        delete temp;
        return tcb;
    }
    else {
        return nullptr; //idle
    }

}

void *Scheduler::Node::operator new(size_t size) {
    return MemoryAllocator::get().mem_alloc(size);
}

void Scheduler::Node::operator delete(void *addr) {
    MemoryAllocator::get().mem_free(addr);
}
