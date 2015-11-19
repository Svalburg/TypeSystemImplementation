#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include "exception.h"
#include "spinlock.h"
#include "memorypool.h"
#include "queue.h"

//#define NO_MEMORYPOOL_ALIGNMENT

namespace bitpowder {
namespace lib {

void MemoryPool::restore(MemoryPoolStatus status) {
    Queue<MemoryPoolItem*> toBeFreed;
    // pass 1, execute delayed items
    while (!delayed.empty() && delayed.back() != status.delayed) {
        MemoryPoolItem* c = delayed.pop();
        if (c->func != NULL)
            (c->func)(c->data);
        toBeFreed.push_back(c);
    }
    checkAssert(!delayed.empty() || status.delayed == nullptr);
    // pass 2, free allocated memory
    while (MemoryPoolItem* c = toBeFreed.pop_front())
        free(c);
}

void MemoryPool::reserve(size_t size) {
}

MemoryPoolStatus MemoryPool::status() {
    return MemoryPoolStatus(delayed.back());
}

void MemoryPool::addMemory(void* data, size_t _size) {
}

void MemoryPool::addMemory(MemoryPool* mp) {
}

MemoryPool::MemoryPool() {
}

MemoryPool::MemoryPool(void* data, size_t size) {
    addMemory(data, size);
}

MemoryPool::~MemoryPool() {
    clear();
}

void MemoryPool::clear() {
    if (!delayed.empty()) {
        MemoryPoolStatus status;
        restore(status);
        checkAssert(delayed.empty());
    }
}

void* MemoryPool::alloc(size_t size, char alignOn, MemoryPoolItem** poolPtr) {
    MemoryPoolItem* i = new (malloc(sizeof(MemoryPoolItem))) MemoryPoolItem();
    checkAssert(i);
    i->func = free;
    i->data = malloc(size);
    delayed.push_back(i);
    if (poolPtr)
        *poolPtr = i;
    return i->data;
}

void* MemoryPool::allocAll(size_t& size, char alignOn) {
    int requestedSize = DEFAULT_SIZE;
    void* retval = alloc(requestedSize, alignOn);
    if (retval)
        size = requestedSize;
    return retval;
}

void* MemoryPool::memdup(const void* src, size_t len, char alignOn) {
    if (src == NULL)
        return NULL;
    void* dst = alloc(len);
    if (dst)
        memcpy(dst, src, len);
    return dst;
}

char* MemoryPool::strndup(const char* src, size_t len) {
    if (src == NULL)
        return NULL;
    char* dst = (char*)alloc(len+1, sizeof(char));
    if (dst) {
        memcpy(dst, src, len);
        dst[len] = '\0';
    }
    return dst;
}

char* MemoryPool::strdup(const char* src) {
    if (src == NULL)
        return NULL;
    //return mpmemdup(src, strlen(src)+1);
    return strndup(src, strlen(src));
}

int MemoryPool::vprintf(char** strPtr, size_t max, const char* format, va_list ap) {
    char* str = (char*)alloc(max);
    checkAssert(str);
    int size = vsnprintf(str, max, format, ap);
    if (size >= ssize_t(max)) {
        str[max-1] = '\0';
    }
    if (strPtr != NULL)
        *strPtr = str;
    return size;
}

int MemoryPool::printf(char** strPtr, size_t max, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    int retval = vprintf(strPtr, max, format, ap);
    va_end(ap);
    return retval;
}

// not terminating the string, useful for generating byte strings
int MemoryPool::vprintf_nt(char** strPtr, size_t max, const char* format, va_list ap) {
    return vprintf(strPtr, max, format, ap);
}

int MemoryPool::printf_nt(char** strPtr, size_t max, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    int retval = vprintf_nt(strPtr, max, format, ap);
    va_end(ap);
    return retval;
}

// clean up all remembered and unused pools
void MemoryPool::vacuum() {
}

MemoryPool::MemoryPool(MemoryPool* from, size_t size) {
    if (from) {
        from->delay((DelayedFunc*)(void (*)(MemoryPool*))&destroy<MemoryPool>, this);
    }
}

MemoryPool::MemoryPool(MemoryPool& from, size_t size) {
    from.delay((DelayedFunc*)(void (*)(MemoryPool*))&destroy<MemoryPool>, this);
}

void* MemoryPool::calloc(size_t size, char alignOn) {
    void* retval = alloc(size, alignOn);
#ifndef EXTRA_SAFE
    memset(retval, 0, size);
#endif
    return retval;
}

}
}

