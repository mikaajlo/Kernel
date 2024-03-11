//
// Created by os on 19/08/23.
//

#include "../h/RiscV.h"

void Riscv::interruptHandler() {
    uint64 sepc = r_sepc();
    uint64 sstatus = r_sstatus();
    uint64 scause = r_scause();
    uint64 code;
    __asm__ volatile("mv %[code], a0" : [code] "=r" (code));
    uint64 a1, a2, a3, a4;
    __asm__ volatile("mv %[a1], a1" : [a1] "=r" (a1));
    __asm__ volatile("mv %[a2], a2" : [a2] "=r" (a2));
    __asm__ volatile("mv %[a3], a3" : [a3] "=r" (a3));
    __asm__ volatile("mv %[a4], a6" : [a4] "=r" (a4));

    if (scause == 8 || scause == 9) {
        if (code == 0x01) {
            uint64 size = a1;
            void* retVal = MemoryAllocator::get().mem_alloc(size);
            __asm__ volatile("mv a0, %[retVal]" : : [retVal] "r" (retVal));
        }
        else if (code == 0x02) {
            void* addr = (void*) a1;
            int retVal = MemoryAllocator::get().mem_free(addr);
            __asm__ volatile("mv a0, %[retVal]" : : [retVal] "r" (retVal));
        }
        else if (code == 0x11) {
            TCB** handle = (TCB**) a1;
            TCB::Body body = (TCB::Body) a2;
            TCB::Argument argument = (TCB::Argument) a3;
            uint64* stack = (uint64*) a4;
            TCB* tcb = TCB::create(body, argument, stack);

            if (!tcb) __asm__ volatile("li a0, -1");
            else {
                *handle = tcb;
                Scheduler::put(*handle);
                __asm__ volatile("li a0, 0");
            }
        }
        else if (code == 0x12) {
            TCB::exit();
        }
        else if (code == 0x13) {
            TCB::dispatch();
        }
        else if (code == 0x14) {
            TCB* handle = (TCB*) a1;
            if (handle) {
                TCB::join(handle);
                __asm__ volatile("li a0, 0");
            }
            else __asm__ volatile("li a0, -1");
        }
        else if (code == 0x21) {
            Sem** handle = (Sem**) a1;
            int value = a2;
            *handle = Sem::create(value);
            if (handle) __asm__ volatile("li a0, 0");
            else __asm__ volatile("li a0, -1");
        }
        else if (code == 0x22) {
            Sem* handle = (Sem*) a1;
            if (handle) {
                delete handle;
                __asm__ volatile("li a0, 0");
            }
            else __asm__ volatile("li a0, -1");
        }
        else if (code == 0x23) {
            Sem* handle = (Sem*) a1;
            if (!handle || TCB::running->closedSem) {
                __asm__ volatile("li a0, -1");
            }
            else {
                handle->wait();
                __asm__ volatile("li a0, 0");
            }
        }
        else if (code == 0x24) {
            Sem* handle = (Sem*) a1;
            if (handle) {
                handle->signal();
                __asm__ volatile("li a0, 0");
            }
            else {
                __asm__ volatile("li a0, -1");
            }
        }
        else if (code ==  0x41) {
            __getc();
        }
        else if (code == 0x42) {
            char c = (char)a1;
            __putc(c);
        }

        Riscv::w_sepc(sepc + 4);
        Riscv::w_sstatus(sstatus);
    }
}

void Riscv::consoleHandler() {
    console_handler();
}

void Riscv::timerHandler() {
    mc_sip(SIP_SSIE);
}

void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("li t0, 256");
    __asm__ volatile("csrc sstatus, t0");
    __asm__ volatile("sret");
}