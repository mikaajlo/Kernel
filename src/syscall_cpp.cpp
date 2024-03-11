//
// Created by os on 23/08/23.
//

#include "../h/syscall_cpp.hpp"

void *operator new(size_t size) {
    return mem_alloc(size);
}

void operator delete(void *addr) {
    mem_free(addr);
}

Thread::Thread(void (*body)(void *), void *arg) {
    this->body = body;
    this->arg = arg;
}

Thread::Thread() {
    body = nullptr;
    arg = nullptr;
}

Thread::~Thread() {
    delete myHandle;
}

int Thread::start() {
    if (body) return thread_create(&myHandle, body, arg);
    else return thread_create(&myHandle, Thread::wrapper,this);
}

void Thread::dispatch() {
    thread_dispatch();
}

void Thread::wrapper(void *thread) {
    ((Thread*)thread)->run();
}

void Thread::join() {
    thread_join(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}
