//
// Created by os on 8/13/23.
//

#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_H
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_H

#include "../lib/hw.h"

struct FreeMemory {
    FreeMemory* next;
    FreeMemory* previous;
    size_t size;
};

class MemoryAllocator {
    FreeMemory* head = nullptr;
    bool first = true;
    void joinNextFree(FreeMemory*);
public:
    static MemoryAllocator& get();
    void* mem_alloc (size_t);
    int mem_free (void*);
};


#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_H
