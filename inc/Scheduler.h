//
// Created by os on 23/08/23.
//

#ifndef PROJECT_BASE_V1_1_SCHEDULER_H
#define PROJECT_BASE_V1_1_SCHEDULER_H

#include "../h/MemoryAllocator.h"

class TCB;

class Scheduler {
    struct Node {
        TCB* thread;
        Node* next = nullptr;

        void *operator new(size_t );
        void operator delete(void*);
    };

    static Node* head;
    static Node* tail;
public:
    static void put(TCB* thread);
    static TCB* get();
};

#endif //PROJECT_BASE_V1_1_SCHEDULER_H
