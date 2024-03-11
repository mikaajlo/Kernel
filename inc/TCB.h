//
// Created by os on 23/08/23.
//

#ifndef PROJECT_BASE_V1_1_TCB_H
#define PROJECT_BASE_V1_1_TCB_H

#include "../lib/hw.h"
#include "../h/Scheduler.h"
#include "../h/RiscV.h"
#include "../h/syscall_c.hpp"

class TCB {
public:
    using Body = void (*)(void*);
    using Argument = void*;

private:
    struct Context {
        uint64 ra, sp;
    };

    Context context;
    uint64* stack;
    Argument argument;
    Body body;
    Sem* sem;

    TCB(Body, Argument, uint64*);

    bool idle = false;

public:
    bool finished = false;
    static TCB* running;
    bool closedSem = false;

    static void yield(Context*, Context*);

    static void initialize();

    static TCB* create(Body, Argument, uint64*);

    static void dispatch();

    static void wrapper();

    static int exit();

    static void join(TCB*);

    static void idleBody(void*);

    void* operator new(size_t);
    void operator delete(void*);
};

#endif //PROJECT_BASE_V1_1_TCB_H
