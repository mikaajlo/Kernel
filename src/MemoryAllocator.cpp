//
// Created by os on 8/13/23.
//

#include "../h/MemoryAllocator.h"

MemoryAllocator &MemoryAllocator::get() {
    static MemoryAllocator instance;
    return instance;
}

void *MemoryAllocator::mem_alloc(size_t size) {
    if (first) {
        first = false;
        head = (FreeMemory*) HEAP_START_ADDR;
        head->size = (uint64)HEAP_END_ADDR - (uint64)HEAP_START_ADDR;
        head->next = nullptr;
        head->previous = nullptr;
    }

    FreeMemory* temp = head;

    size += sizeof(FreeMemory);
    size = (size % MEM_BLOCK_SIZE) ?
           ((size / MEM_BLOCK_SIZE + 1) * MEM_BLOCK_SIZE) : size;

    while (temp && size > temp->size) temp = temp->next;
    if (!temp) return nullptr;

    if (temp->size - size > 0) {
        FreeMemory* newFreeMem = (FreeMemory*)((uint64)temp + size);

        if (temp->previous) temp->previous->next = newFreeMem;
        else head = newFreeMem;
        if (temp->next) temp->next->previous = newFreeMem;
        newFreeMem->previous = temp->previous;
        newFreeMem->next = temp->next;
        newFreeMem->size = temp->size - size;
    }
    else {
        if (temp->previous) temp->previous->next = temp->next;
        else head = temp->next;
        if (temp->next) temp->next->previous = temp->previous;
    }
    temp->size = size;
    temp->next = temp->previous = nullptr;
    return (void*)((uint64)temp + sizeof(FreeMemory));
}

int MemoryAllocator::mem_free(void *addr) {
    addr =  (void*)((uint64)addr - sizeof(FreeMemory));
    if ((uint64*)addr < (uint64*)HEAP_START_ADDR || (uint64*)addr > (uint64*)HEAP_END_ADDR || first)  {
        return -1;
    }

    FreeMemory* temp = nullptr;
    if (head && (uint64*)addr >= (uint64*)head) {
        for (temp = head; temp->next && (uint64*)addr > (uint64*)temp->next; temp = temp->next);
    }

    FreeMemory* newFreeMem = (FreeMemory*)addr;
    newFreeMem->previous = temp;

    if (temp) {
        if ((uint64)temp + temp->size > (uint64)addr) return -1;
        newFreeMem->next = temp->next;
    }
    else newFreeMem->next = head;
    if (newFreeMem->next) newFreeMem->next->previous = newFreeMem;
    if (temp) temp->next = newFreeMem;
    else head = newFreeMem;

    joinNextFree(newFreeMem);
    joinNextFree(temp);

    return 0;
}

void MemoryAllocator::joinNextFree(FreeMemory* freeMem) {
    if (freeMem) {
        if (freeMem->next && (uint64) freeMem + freeMem->size == (uint64) freeMem->next) {
            freeMem->size += freeMem->next->size;
            freeMem->next = freeMem->next->next;
            if (freeMem->next) freeMem->next->previous = freeMem;
        }
    }
}