//
// Created by os on 08/09/23.
//

#ifndef PROJECT_BASE_V1_1_SEM_H
#define PROJECT_BASE_V1_1_SEM_H

#include "../h/TCB.h"



class Sem {
    struct Blocked {
        TCB* tcb;
        Blocked* next = nullptr;

        void *operator new(size_t );
        void operator delete(void*);
    };
    Blocked* head = nullptr;
    Blocked* tail = nullptr;
    int value;

public:
    Sem(int);
    ~Sem();

    static Sem* create(int);
    void wait();
    void signal();

    void *operator new(size_t );
    void operator delete(void*);


};

#endif //PROJECT_BASE_V1_1_SEM_H
