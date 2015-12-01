/**
Copyright 2010-2015 Bernard van Gastel, bvgastel@bitpowder.com.
This file is part of Bit Powder Libraries.

Bit Powder Libraries is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Bit Powder Libraries is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Bit Powder Libraries.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifdef MEMORY_DEBUG
#include "memorypool.debug.cpp"
#else

#include "memorypool.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <cstdint>

#ifndef NCONCURRENCY
#include "spinlock.h"
#endif

#ifdef __WIN32__
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#endif

#ifdef NDEBUG
#define EXTRA_SAFE
#endif

/*
 * TODO:
 * - reserve(int chunks, int maxSizeOfItem): make users of reserve specify more clearly what they need
 * - on a large allocation, allocate a power of two and returnUnused() the remaining memory
 * - for MemoryPoolPage, maybe use CacheInstances, instead of own implementation (but now, all the unused MemoryPools are put in the pool with one locking operation)
 */

/**
 * Two important situations that need to be taken into account when modifying the implementation of MemoryPool:
 * - when restoring/clearing a MemoryPool containing a STL container with custom allocator: the memory freed/deallocted
 * by the STL container must not be re-added to the MemoryPool, as the MemoryPage containing this memory is going to be
 * deallocted
 * - if new memory is allocated (especially memory that is allocated out of band of the memory pool system), needs to
 * be deallocted after all the delayed objects are executed, as a STL container can access this memory during its
 * destructor phase.
 */

namespace bitpowder {
namespace lib {

void MemoryPool::restore(const MemoryPoolStatus& status) {
    rewinding = true;
    MemoryPoolPageBase* currentMemoryPool = nullptr;
    Deque<MemoryPoolPageBase*> currentAll;
    currentAll.swap(all);
    std::swap(currentMemoryPool, current);

    // PASS 2, dispose of pages (not earlier, because a delay function of an 'earlier' memorypool can depend on memory of a 'later' memorypool...)
    Queue<MemoryPoolPageBase*,MemoryPoolPageBase,&MemoryPoolPageBase::prev> readd;
    Deque<MemoryPoolPageBase*> toNextPages;
    while (currentMemoryPool && currentMemoryPool != status.current) {
        MemoryPoolPageBase* prev = currentAll.prev(currentMemoryPool);
        assert(currentAll.front() == currentMemoryPool || currentAll.front() != currentAll.back());
        currentAll.erase(currentMemoryPool);
        if (currentMemoryPool->fixedSize) {
            readd.push(currentMemoryPool);
        } else {
            toNextPages.push_front(currentMemoryPool);
        }
        currentMemoryPool = prev;
    }
    assert(currentMemoryPool == status.current);

    currentAll.insert(currentAll.next(currentMemoryPool), std::move(toNextPages));

    // rewind nextPages, as these are never allocated from the global pool, we don't have to readded them
    while (currentAll.back() && currentAll.back() != status.nextPage && currentAll.back() != currentMemoryPool)
        currentAll.pop_back();

    // reset available to previous value
    if (status.current) {
        assert(status.current->available <= status.available);
        status.current->available = status.available;
    }

    // PASS 3, if new memorypools are made during executing this function, append the old linked list to the last one of the new one
    if (all.empty()) {
        current = status.current;
    } else {
        // do not modifiy 'current'!
        Deque<MemoryPoolPageBase*> active = all.split(current); // split the part currently in use
        currentAll.insert(currentAll.next(currentMemoryPool), std::move(active));
        currentAll.push_back(std::move(all));
    }
    all = std::move(currentAll);

    // this should be done fairly late, as here memory gets deallocated, and in that memory MemoryPoolPages can exists (that are in the linked lists that are traversed above)
    while (memoryObjects.back() != status.memoryObjects) {
        auto object = memoryObjects.pop_back();
        object->execute();
    }

    // add all the memorypools to be recycled in one operation to the pool
    // this should be executed as last, as 'memoryObjects' objects can live inside these pools
    if (!readd.empty()) {
        for (const auto& page : readd)
            page->reset();
        MemoryPoolPageDefaultSize::reuse(readd);
    }

    if (current) {
#ifdef EXTRA_SAFE
        assert(current->available <= current->size);
        bzero(&((char*)current->data)[current->size - current->available], current->available);
#endif
    }

    rewinding = false;
}

MemoryPool::~MemoryPool() {
    rewinding = true;
    MemoryPoolPageBase* currentMemoryPool = nullptr;
    Deque<MemoryPoolPageBase*> currentAll;
    currentAll.swap(all);
    std::swap(currentMemoryPool, current);

    // PASS 2, dispose of pages (not earlier, because a delay function of an 'earlier' memorypool can depend on memory of a 'later' memorypool...)
    Queue<MemoryPoolPageBase*,MemoryPoolPageBase,&MemoryPoolPageBase::prev> readd;
    while (currentMemoryPool) {
        MemoryPoolPageBase* prev = currentAll.prev(currentMemoryPool);
        assert(currentAll.front() == currentMemoryPool || currentAll.front() != currentAll.back());
        currentAll.erase(currentMemoryPool);
        if (currentMemoryPool->fixedSize) {
            readd.push(currentMemoryPool);
        }
        currentMemoryPool = prev;
    }
    assert(currentMemoryPool == nullptr);

    // PASS 3, if new memorypools are made during executing this function, append the old linked list to the last one of the new one
    // NOP

    // this should be done fairly late, as here memory gets deallocated, and in that memory MemoryPoolPages can exists (that are in the linked lists that are traversed above)
    while (auto object = memoryObjects.pop_back())
        object->execute();

    // add all the memorypools to be recycled in one operation to the pool
    // this should be executed as last, as 'memoryObjects' objects can live inside these pools
    if (!readd.empty()) {
        for (const auto& page : readd)
            page->reset();
        MemoryPoolPageDefaultSize::reuse(readd);
    }
}

MemoryPoolStatus MemoryPool::status() {
    MemoryPoolStatus retval(current, all.back());
    if (retval.current) {
        retval.available = retval.current->available;
        retval.memoryObjects = memoryObjects.back();
    }
    return retval;
}

bool MemoryPool::addMemory(void* data, size_t _size) {
    if (data == nullptr)
        return false;

    // make multiple of alignment, so MemoryPoolPageVariableSize start at a correct alignment
    size_t extra = ((intptr_t)data) & (DEFAULT_ALIGN_ON - 1);
    if (extra) {
        if (DEFAULT_ALIGN_ON-extra > _size) {
            data = nullptr;
            _size = 0;
        } else {
            data = &((char*)data)[DEFAULT_ALIGN_ON-extra];
            _size -= (DEFAULT_ALIGN_ON-extra);
        }
    }

    if (data == nullptr || _size <= sizeof(MemoryPoolPageVariableSize)) // || size > USHRT_MAX /* length of short */)
        return false;

#ifdef EXTRA_SAFE
    bzero(data, _size);
#endif

    MemoryPoolPageBase* page = new (data) MemoryPoolPageVariableSize(_size - sizeof(MemoryPoolPageVariableSize));
    all.push_back(page);
    if (!current)
        current = page;
    return true;
}

void MemoryPool::addMemory(MemoryPool* mp) {
    if (mp) {
        size_t size = 0;
        void* data = mp->allocAll(size);
        if (data && size > 0)
            addMemory(data, size);
    }
}

// allocs new pool
MemoryPoolPageBase* MemoryPool::allocExtra() {
    if (current && all.next(current)) {
        return current = all.next(current);
    }
    assert(all.back() == current);
    MemoryPoolPageBase* page = new MemoryPoolPageDefaultSize();

    // really add the memory to the pools
    assert(page->available == page->size);
    assert(page->size == MEMORYPOOL_SIZE_DEFAULT_SIZE);
    assert(page->next == nullptr);
    assert(page->prev == nullptr);
    assert(page->fixedSize);

    all.insert(all.next(current), page);
    return current = page;
}

MemoryPool::MemoryPool() {
}

MemoryPool::MemoryPool(size_t size, void* data) {
    addMemory(data, size);
}

void MemoryPool::clear() {
    assert(this);
    if (current) {
        MemoryPoolStatus status;
        restore(status);
        assert(current == nullptr);
    }
}

bool MemoryPoolPageBase::fitsIn(MemoryPoolPageBase* pool, size_t size, size_t alignOn, size_t& padding) {
    if (!pool)
        return false;
    void* potentialNextPointer = &((char*)pool->data)[pool->size - pool->available];
    padding = (intptr_t)(potentialNextPointer) & (alignOn - 1);
    if (padding > 0)
        padding = alignOn - padding;
    return pool->available >= size + padding;
}

void* MemoryPool::alloc(size_t size, size_t alignOn, MemoryPoolPageBase** pool) {
    checkAssert(size >= 0 && size <= 1*1024*1024*1024); // less as 1 Gb, otherwise probably an error
    if (size == 0)
        return nullptr;

    size_t padding = 0;

    // do not facilitate large requests to the pool, except if there is (because of a super memorypool) room available
    if (size > MEMORYPOOL_SIZE/2 && !MemoryPoolPageBase::fitsIn(current, size, alignOn, padding)) {
        std::size_t bytes = size+sizeof(MemoryPoolPageVariableSize);
        char* data = (char*)malloc_with_minimal_size(bytes);
        delay([data] {free(data);});
        addMemory(data, bytes); // add the page to the back of all pages
        allocExtra(); // so we know at least one page needs to be alloced (because !fitsIn)
    }
    // search for pool with enough space
    while (!MemoryPoolPageBase::fitsIn(current, size, alignOn, padding))
        current = allocExtra();
    assert(current->available >= 0);
    assert(size <= current->available);
    // alloc
    void* data = &((char*)current->data)[current->size - current->available + padding];
    current->available -= size + padding;
    assert(current->available >= 0);
    if (pool != nullptr)
        *pool = current;
    assert((intptr_t)(data) % alignOn == 0);
    return data;
}

void* MemoryPool::allocAll(size_t& size, size_t alignOn) {
    size_t padding = 0;
    MemoryPoolPageBase::fitsIn(current, size, alignOn, padding);
    long requestedSize = current ? current->available - padding : 0;
    void* retval = requestedSize > 0 ? alloc(requestedSize, alignOn) : nullptr;
    if (retval)
        size = requestedSize;
    assert((intptr_t)(retval) % alignOn == 0);
    return retval;
}

void* MemoryPool::memdup(const void* src, size_t len, size_t alignOn) {
    if (src == nullptr)
        return nullptr;
    void* dst = alloc(len, alignOn);
    if (dst)
        memcpy(dst, src, len);
    return dst;
}

char* MemoryPool::strndup(const char* src, size_t len) {
    if (src == nullptr)
        return nullptr;
    char* dst = (char*)alloc(len+1, sizeof(char));
    if (!dst)
        return nullptr;
    memcpy(dst, src, len);
    dst[len] = '\0';
    return dst;
}

char* MemoryPool::strdup(const char* src) {
    if (src == nullptr)
        return nullptr;
    //return mpmemdup(src, strlen(src)+1);
    return strndup(src, strlen(src));
}

int MemoryPool::vprintf(char** strPtr, size_t max, const char* format, va_list ap) {
    if (max == 0)
        return -1;
    MemoryPoolPageBase* p = nullptr;
    int size = 0;
    char* str = (char*)alloc(max, sizeof(char), &p);
    if (str) {
        //check(str != nullptr);
        //check(p != nullptr);
        size = vsnprintf(str, max, format, ap);
        // check for error
        if (size < 0) {
            returnUnused(str, max);
            if (strPtr != nullptr)
                *strPtr = nullptr;
            return size;
        }
        // check if there was enough size in the string, otherwise for safety terminate string
        if ((size_t)size >= max) {
            str[max-1] = '\0';
        } else {
            // some space is left!
            returnUnused(str+size+1, max-size-1);
        }
    }
    if (strPtr != nullptr)
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
    if (max == 0)
        return -1;
    MemoryPoolPageBase* p = nullptr;
    char* str = (char*)alloc(max, sizeof(char), &p);
    //check(str != nullptr);
    int size = 0;
    //check(p != nullptr);
    if (str) {
        size = vsnprintf(str, max, format, ap);
        // check for error
        if (size < 0) {
            // check if pointer is really alloced inside memorypool
            returnUnused(str, max);
            if (strPtr != nullptr)
                *strPtr = nullptr;
            return size;
        }
        if (((size_t)size) < max) {
            // some space is left!
            returnUnused(str+size, max-size);
        }
    }
    if (strPtr != nullptr)
        *strPtr = str;
    return size;
}

bool MemoryPool::returnUnused(char* buffer, size_t len) {
    if (current && buffer+len == next(sizeof(char))) {
        current->available += len;
        return true;
    } else {
        return addMemory(buffer, len);
        //return false;
    }
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
    MemoryPoolPageDefaultSize::purge();
}

MemoryPool::MemoryPool(MemoryPool* from, size_t size) {
    if (from && from->available() >= size) {
        void* data = from->alloc(size+sizeof(MemoryPoolPageVariableSize));
        addMemory(data, size+sizeof(MemoryPoolPageVariableSize));
    }
}

MemoryPool::MemoryPool(MemoryPool& from, size_t size) {
    if (from.available() >= size) {
        void* data = from.alloc(size+sizeof(MemoryPoolPageVariableSize));
        addMemory(data, size+sizeof(MemoryPoolPageVariableSize));
    }
}

void* MemoryPool::calloc(size_t size, size_t alignOn) {
    void* retval = alloc(size, alignOn);
    bzero(retval, size);
    return retval;
}

void MemoryPool::reserve(std::size_t bytes, size_t alignOn) {
    if (bytes == 0)
        return;

    // do not facilitate large requests to the pool
    size_t padding = 0;
    if (bytes > MEMORYPOOL_SIZE/2 && !MemoryPoolPageBase::fitsIn(current, bytes, alignOn, padding)) {
        void* data = malloc_with_minimal_size(bytes);
        addMemory(data, bytes);
        delay([data] {free(data);});
        return;
    }
    while (!MemoryPoolPageBase::fitsIn(current, bytes, alignOn, padding))
        allocExtra();
}

MemoryPoolPageBase::MemoryPoolPageBase(void* data, unsigned int size, bool fixedSize) : data((char*)data), size(size), available(size), fixedSize(fixedSize) {
}

void MemoryPoolPageBase::reset() {
    assert(size == MEMORYPOOL_SIZE-sizeof(MemoryPoolPageBase));
    assert(available <= size);
#ifdef EXTRA_SAFE
    bzero(data, size-available);
#endif
    available = size;
}

MemoryPoolPageDefaultSize::MemoryPoolPageDefaultSize() : MemoryPoolPageBase(store, MEMORYPOOL_SIZE_DEFAULT_SIZE, true) {
}

MemoryPoolPageVariableSize::MemoryPoolPageVariableSize(unsigned int size) : MemoryPoolPageBase(store, size, false) {
}

}
}
#endif
